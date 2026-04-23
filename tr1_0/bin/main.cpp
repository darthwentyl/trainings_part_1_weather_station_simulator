#include <iostream>
#include <memory>
#include <chrono>
#include <thread>

#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>

#include <logger/Log.h>
#include <ipc/SharedSegmentMemoryIpc.h>
#include <ipc/SharedSegmentSemaphoreIpc.h>
#include <proc_managers/ReaderManager.h>
#include <proc_managers/WriterManager.h>
#include <proc_managers/workers/WeatherWorker.h>
#include <proc_managers/workers/TemperatureWorker.h>
#include <proc_managers/workers/PressureWorker.h>
#include <proc_managers/workers/ExitWorker.h>

using namespace mw::ipc;
using namespace mw::proc_managers;
using namespace mw::proc_managers::workers;

namespace {
    constexpr const char* data_sem_name = "data.sem";
    constexpr const char* reader_sem_name = "reader.sem";
    constexpr const char* mem_name = "weather_data_memory";
    constexpr const std::size_t mem_size = 128;
    constexpr const std::size_t reader_nums = 2;
    constexpr const std::size_t file_max_line_numbers = 10;
} // anonymous

int main() {
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

    pid_t writer_weather_pid = fork();
    if (writer_weather_pid == 0) {
        try {
            SharedSegmentSemaphoreIpc data_sem{std::string{data_sem_name}, EUsageShmSegment::CLIENT};
            SharedSegmentSemaphoreIpc reader_sem{std::string{reader_sem_name}, EUsageShmSegment::CLIENT};
            SharedSegmentMemoryIpc mem{std::string{mem_name}, mem_size, EUsageShmSegment::CLIENT};
            WeatherWorker weather_worker{mem};
            WriterManager manager{reader_nums, data_sem, reader_sem, weather_worker};

            manager.loop();

        } catch(const std::exception& e) {
            ERROR(e.what());
        }
        exit(0);
    }

    pid_t reader_temperature_pid = fork();
    if (reader_temperature_pid == 0) {
        try {
            SharedSegmentSemaphoreIpc data_sem{std::string{data_sem_name}, EUsageShmSegment::CLIENT};
            SharedSegmentSemaphoreIpc reader_sem{std::string{reader_sem_name}, EUsageShmSegment::CLIENT};
            SharedSegmentMemoryIpc mem{std::string{mem_name}, mem_size, EUsageShmSegment::CLIENT};
            TemperatureWorker temperature_worker{mem, file_max_line_numbers};
            ReaderManager manager{data_sem, reader_sem, temperature_worker};

            manager.loop();

        } catch(const std::exception& e) {
            ERROR(e.what());
        }
        exit(0);
    }

    pid_t reader_pressure_pid = fork();
    if (reader_pressure_pid == 0) {
        try {
            SharedSegmentSemaphoreIpc data_sem{std::string{data_sem_name}, EUsageShmSegment::CLIENT};
            SharedSegmentSemaphoreIpc reader_sem{std::string{reader_sem_name}, EUsageShmSegment::CLIENT};
            SharedSegmentMemoryIpc mem{std::string{mem_name}, mem_size, EUsageShmSegment::CLIENT};
            PressureWorker pressure_worker{mem, file_max_line_numbers};
            ReaderManager manager{data_sem, reader_sem, pressure_worker};

            manager.loop();

        } catch(const std::exception& e) {
            ERROR(e.what());
        }
        exit(0);
    }

    std::cout << "Press any key for exit..." << std::endl;
    std::cin.get();
    try {

        SharedSegmentSemaphoreIpc data_sem{std::string{data_sem_name}, EUsageShmSegment::CLIENT};
        SharedSegmentSemaphoreIpc reader_sem{std::string{reader_sem_name}, EUsageShmSegment::CLIENT};
        SharedSegmentMemoryIpc mem{std::string{mem_name}, mem_size, EUsageShmSegment::CLIENT};

        ExitWorker exit_worker{mem};
        WriterManager manager{reader_nums, data_sem, reader_sem, exit_worker};

        manager.loop();

    } catch (const std::exception& e) {
        ERROR(e.what());
    }

    for (size_t n = 0; n < 4; ++n) {
        wait(NULL);
    }

    return 0;
}