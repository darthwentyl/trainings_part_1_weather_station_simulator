#include <proc_managers/workers/TemperatureWorker.h>
#include <proc_managers/workers/WeatherData.h>

#include <logger/Log.h>
#include <ipc/IIpc.h>

namespace mw { namespace proc_managers { namespace workers {

using namespace mw::ipc;

TemperatureWorker::TemperatureWorker(IIpc& ipcMemory,const std::string& dataFile, const std::size_t bufferSize) :
    Worker{ipcMemory},
    writer{dataFile, bufferSize}
{}

void TemperatureWorker::processData() {
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
        DEBUG("Received temperature: " << data.getTemperature() << " [C]");
        writer.write<double>(data.getTemperature());
    }
}

} } } // mw::proc_managers::workers