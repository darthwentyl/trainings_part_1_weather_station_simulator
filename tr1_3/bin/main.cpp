#include <iostream>
#include <memory>
#include <chrono>
#include <thread>

#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>

#include <logger/Log.h>

#include <ipc/PipeStreamIpc.h>
#include <ipc/SharedSegmentMemoryIpc.h>
#include <ipc/SharedSegmentSemaphoreIpc.h>
#include <ipc/TelnetIpc.h>

#include <proc_managers/ReaderManager.h>
#include <proc_managers/WriterManager.h>
#include <proc_managers/workers/WeatherWorker.h>
#include <proc_managers/workers/TemperatureWorker.h>
#include <proc_managers/workers/PressureWorker.h>
#include <proc_managers/workers/ExitWorker.h>
#include <proc_managers/workers/GnuplotWorker.h>
#include <proc_managers/workers/UserCmdWorker.h>

#include <helpers/GnuplotDescriptionBuilder.h>

using namespace mw::ipc;
using namespace mw::proc_managers;
using namespace mw::proc_managers::workers;
using namespace mw::helpers;

namespace {
    constexpr const char* data_sem_name = "data.sem";
    constexpr const char* reader_sem_name = "reader.sem";
    constexpr const char* mem_name = "weather_data_memory";
    constexpr const std::size_t mem_size = 128;
    constexpr const std::size_t reader_nums = 5;
    constexpr const std::size_t writer_nums = 2;
    constexpr const std::size_t file_max_line_numbers = 10;
    constexpr const char* temperature_dat = "temperature.dat";
    constexpr const char* pressure_dat = "pressure.dat";
    constexpr const char* gnuplot_cmd = "gnuplot";

    static bool isCriticalExited = false;
    static pid_t parent_pid = 0;
} // anonymous

void critical_exit([[maybe_unused]] int sig) {
    DEBUG("Signal received: " << sig << " isCriticalExited: " << std::boolalpha << isCriticalExited);
    if (!isCriticalExited) {
        SharedSegmentSemaphoreIpc data_sem{std::string{data_sem_name}, EUsageShmSegment::CLIENT};
        SharedSegmentSemaphoreIpc reader_sem{std::string{reader_sem_name}, EUsageShmSegment::CLIENT};
        SharedSegmentMemoryIpc mem{std::string{mem_name}, mem_size, EUsageShmSegment::CLIENT};
        DEBUG("mem/sem prepared");
        data_sem.open();
        reader_sem.open();
        ExitWorker exit_worker{mem};

        DEBUG("ExitWorker created");
        exit_worker.startWorking();
        exit_worker.processData();
        data_sem.post();
        isCriticalExited = true;
    }
    INFO("Critical exit done... Please press enter to exit...");
}

int main() {
    DEBUG("Start parent...");
    SharedSegmentSemaphoreIpc data_main_sem{std::string{data_sem_name}, EUsageShmSegment::CREATOR};
    SharedSegmentSemaphoreIpc reader_main_sem{std::string{reader_sem_name}, EUsageShmSegment::CREATOR};
    SharedSegmentMemoryIpc main_mem{std::string{mem_name}, mem_size, EUsageShmSegment::CREATOR};

    try {
        data_main_sem.open(1);
        reader_main_sem.open(0);
        main_mem.open();
    } catch(const std::exception& e) {
        ERROR(e.what());
        exit(-1);
    }

    signal(SIGINT, critical_exit);
    parent_pid = getpid();

    pid_t writer_weather_pid = fork();
    if (writer_weather_pid == 0) {
        try {
            DEBUG("Start weather writer...");
            SharedSegmentSemaphoreIpc data_sem{std::string{data_sem_name}, EUsageShmSegment::CLIENT};
            SharedSegmentSemaphoreIpc reader_sem{std::string{reader_sem_name}, EUsageShmSegment::CLIENT};
            SharedSegmentMemoryIpc mem{std::string{mem_name}, mem_size, EUsageShmSegment::CLIENT};
            WeatherWorker weather_worker{mem};
            WriterManager manager{parent_pid, reader_nums, data_sem, reader_sem, weather_worker};

            manager.loop();

        } catch(const std::exception& e) {
            ERROR(e.what());
        }
        exit(0);
    }

    pid_t reader_temperature_pid = fork();
    if (reader_temperature_pid == 0) {
        try {
            DEBUG("Start temperature reader...");
            SharedSegmentSemaphoreIpc data_sem{std::string{data_sem_name}, EUsageShmSegment::CLIENT};
            SharedSegmentSemaphoreIpc reader_sem{std::string{reader_sem_name}, EUsageShmSegment::CLIENT};
            SharedSegmentMemoryIpc mem{std::string{mem_name}, mem_size, EUsageShmSegment::CLIENT};
            TemperatureWorker temperature_worker{mem, temperature_dat, file_max_line_numbers};
            ReaderManager manager{parent_pid, data_sem, reader_sem, temperature_worker};

            manager.loop();

        } catch(const std::exception& e) {
            ERROR(e.what());
        }
        exit(0);
    }

    pid_t reader_pressure_pid = fork();
    if (reader_pressure_pid == 0) {
        try {
            DEBUG("Start pressure reader...");
            SharedSegmentSemaphoreIpc data_sem{std::string{data_sem_name}, EUsageShmSegment::CLIENT};
            SharedSegmentSemaphoreIpc reader_sem{std::string{reader_sem_name}, EUsageShmSegment::CLIENT};
            SharedSegmentMemoryIpc mem{std::string{mem_name}, mem_size, EUsageShmSegment::CLIENT};
            PressureWorker pressure_worker{mem, pressure_dat, file_max_line_numbers};
            ReaderManager manager{parent_pid, data_sem, reader_sem, pressure_worker};

            manager.loop();

        } catch(const std::exception& e) {
            ERROR(e.what());
        }
        exit(0);
    }

    pid_t temperature_gnuplot_pid = fork();
    if (temperature_gnuplot_pid == 0) {
        try {
            DEBUG("Start temperature gnuplot...");
            SharedSegmentSemaphoreIpc data_sem{std::string{data_sem_name}, EUsageShmSegment::CLIENT};
            SharedSegmentSemaphoreIpc reader_sem{std::string{reader_sem_name}, EUsageShmSegment::CLIENT};
            SharedSegmentMemoryIpc mem{std::string{mem_name}, mem_size, EUsageShmSegment::CLIENT};
            PipeStreamIpc gnuplot_pipe{gnuplot_cmd, EPipeMode::WRITE};
            GnuplotDescription description = GnuplotDescriptionBuilder()
                .dataFile(temperature_dat)
                .width(800)
                .height(600)
                .title("Temperature sensors measurement")
                .xLabel("n")
                .yLabel("T[C]")
                .grid(true)
                .build();

            GnuplotWorker temperature_gnuplot_worker{mem, gnuplot_pipe, description};
            ReaderManager manager{parent_pid, data_sem, reader_sem, temperature_gnuplot_worker};

            manager.loop();
        } catch(const std::exception& e) {
            ERROR(e.what());
        }
        exit(0);
    }

    pid_t pressure_gnuplot_pid = fork();
    if (pressure_gnuplot_pid == 0) {
        try {
            DEBUG("Start pressure gnuplot...");
            SharedSegmentSemaphoreIpc data_sem{std::string{data_sem_name}, EUsageShmSegment::CLIENT};
            SharedSegmentSemaphoreIpc reader_sem{std::string{reader_sem_name}, EUsageShmSegment::CLIENT};
            SharedSegmentMemoryIpc mem{std::string{mem_name}, mem_size, EUsageShmSegment::CLIENT};
            PipeStreamIpc gnuplot_pipe{gnuplot_cmd, EPipeMode::WRITE};
            GnuplotDescription description = GnuplotDescriptionBuilder()
                .dataFile(pressure_dat)
                .width(800)
                .height(600)
                .title("Pressure sensors measurement")
                .xLabel("n")
                .yLabel("P[hPa]")
                .grid(true)
                .build();

            GnuplotWorker pressure_gnuplot_worker{mem, gnuplot_pipe, description};
            ReaderManager manager{parent_pid, data_sem, reader_sem, pressure_gnuplot_worker};

            manager.loop();
        } catch(const std::exception& e) {
            ERROR(e.what());
        }
        exit(0);

    }

    pid_t user_command_pid = fork();
    if (user_command_pid == 0) {
        try {
            DEBUG("Start user command...");
            SharedSegmentSemaphoreIpc data_sem{std::string{data_sem_name}, EUsageShmSegment::CLIENT};
            SharedSegmentSemaphoreIpc reader_sem{std::string{reader_sem_name}, EUsageShmSegment::CLIENT};
            SharedSegmentMemoryIpc mem{std::string{mem_name}, mem_size, EUsageShmSegment::CLIENT};
            TelnetIpc socket_telnet{12345};

            UserCmdWorker user_cmd_worker{mem, socket_telnet, file_max_line_numbers};
            ReaderManager manager{parent_pid, data_sem, reader_sem, user_cmd_worker};

            manager.loop();
        } catch(const std::exception& e) {
            ERROR(e.what());
        }
        exit(0);
    }

    INFO("Press enter for exit...");
    std::cin.get();
    if (!isCriticalExited) {
        try {
            SharedSegmentSemaphoreIpc data_sem{std::string{data_sem_name}, EUsageShmSegment::CLIENT};
            SharedSegmentSemaphoreIpc reader_sem{std::string{reader_sem_name}, EUsageShmSegment::CLIENT};
            SharedSegmentMemoryIpc mem{std::string{mem_name}, mem_size, EUsageShmSegment::CLIENT};

            ExitWorker exit_worker{mem};
            WriterManager manager{parent_pid, reader_nums, data_sem, reader_sem, exit_worker};

            manager.loop();

        } catch (const std::exception& e) {
            ERROR(e.what());
        }
    }


    for (size_t n = 0; n < reader_nums + writer_nums; ++n) {
        wait(NULL);
    }

    return 0;
}