#include <proc_managers/workers/TemperatureWorker.h>
#include <proc_managers/workers/WeatherData.h>

#include <logger/Log.h>
#include <ipc/IIpc.h>

namespace mw { namespace proc_managers { namespace workers {

constexpr const char* TEMPERATURE_FILE = "temperature.dat";

TemperatureWorker::TemperatureWorker(ipc::IIpc& ipcData, const std::size_t bufferSize) :
    Worker{false},
    ipcData{ipcData},
    writer{TEMPERATURE_FILE, bufferSize}
{}

void TemperatureWorker::startWorking() {
    if (isWorking()) {
        INFO("Worker has already started");
        return;
    }

    ipcData.open();
    setWorkingState(true);
}

void TemperatureWorker::processData() {
    if (!isWorking()) {
        INFO("Worker has not started yet");
        return;
    }

    WeatherData data;
    data.deserialize(ipcData.read());
    DEBUG("Received temperature: " << data.getTemperature() << " [C]");
    writer.write<double>(data.getTemperature());
}

void TemperatureWorker::stopWorking() {
    if (!isWorking()) {
        INFO("Worker has already stopped");
        return;
    }

    ipcData.close();
    setWorkingState(false);
}

} } } // mw::proc_managers::workers