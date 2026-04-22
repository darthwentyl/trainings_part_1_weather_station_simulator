#include <proc_managers/workers/PressureWorker.h>
#include <proc_managers/workers/WeatherData.h>

#include <logger/Log.h>
#include <ipc/IIpc.h>

namespace mw { namespace proc_managers { namespace workers {

constexpr const char* PRESSURE_FILE = "pressure.dat";

using namespace mw::ipc;

PressureWorker::PressureWorker(IIpc& ipcData, const std::size_t bufferSize) :
    Worker{false},
    ipcData{ipcData},
    writer{PRESSURE_FILE, bufferSize}
{}

void PressureWorker::startWorking() {
    if (isWorking()) {
        INFO("Worker has already started");
        return;
    }

    ipcData.open();
    setWorkingState(true);
}

void PressureWorker::processData() {
    if (!isWorking()) {
        INFO("Worker has not started yet");
        return;
    }

    const std::string msg = ipcData.read();
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

void PressureWorker::stopWorking() {
    if (!isWorking()) {
        INFO("Worker has already stopped");
        return;
    }
    ipcData.close();
    setWorkingState(false);
}

} } } // mw::proc_managers::workers