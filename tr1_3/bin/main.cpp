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
#include <ipc/SocketIpc.h>

#include <proc_managers/ReaderManager.h>
#include <proc_managers/WriterManager.h>
#include <proc_managers/workers/WeatherWorker.h>
#include <proc_managers/workers/TemperatureWorker.h>
#include <proc_managers/workers/PressureWorker.h>
#include <proc_managers/workers/ExitWorker.h>
#include <proc_managers/workers/GnuplotWorker.h>

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
    constexpr const std::size_t reader_nums = 4;
    constexpr const std::size_t file_max_line_numbers = 10;
    constexpr const char* temperature_dat = "temperature.dat";
    constexpr const char* pressure_dat = "pressure.dat";
    constexpr const char* gnuplot_cmd = "gnuplot";
} // anonymous

int main() {
    try {
        auto socketIpc = SocketIpc{12345};
        socketIpc.open();
        std::string data;
        const std::string disconnected{"Client disconnected"};
        const std::string exit_prog{"exit"};
        while (true) {
            data = socketIpc.read();
            INFO("data: " << data << " data.size(): " << data.size());
            if (data == disconnected || data == exit_prog) {
                break;
            }
            data = socketIpc.write("ping");
        }
    } catch (const std::exception& e) {
        ERROR(e.what());
    }

    return 0;
}