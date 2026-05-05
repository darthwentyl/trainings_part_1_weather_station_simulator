#include <proc_managers/workers/PressureWorker.h>
#include <proc_managers/workers/WeatherData.h>

#include <logger/Log.h>
#include <ipc/IIpc.h>

namespace mw { namespace proc_managers { namespace workers {

constexpr const char* PRESSURE_FILE = "pressure.dat";

using namespace mw::ipc;

PressureWorker::PressureWorker(IIpc& ipcMemory, const std::size_t bufferSize) :
    Worker{ipcMemory},
    writer{PRESSURE_FILE, bufferSize}
{}

void PressureWorker::processData() {
    if (!isWorking()) {
        INFO("Worker has not started yet");
        return;
    }

    const std::string msg = ipcMem().read();
    DEBUG("read: " << msg);

    if (msg == "exit") {
        stopWorking();
    } else {
        WeatherData data;
        data.deserialize(msg);
        DEBUG("Received pressure: " << data.getPressure() << " [hPa]");
        writer.write<double>(data.getPressure());
    }
}

} } } // mw::proc_managers::workers
