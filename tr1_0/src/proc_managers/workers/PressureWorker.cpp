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
    // TODO: only for test without close procedure
    static std::size_t loop = 0;
    // TODO:


    if (!isWorking()) {
        INFO("Worker has not started yet");
        return;
    }

    WeatherData data;
    data.deserialize(ipcData.read());
    DEBUG("Received pressure: " << data.getPressure() << " [hPa]");
    writer.write<double>(data.getPressure());

    // TODO: only for test without close procedure
    loop++;
    if (loop == 20) {
        stopWorking();
        loop = 0;
    }
    // TODO:
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