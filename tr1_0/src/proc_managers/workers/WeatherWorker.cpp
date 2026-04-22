#include <proc_managers/workers/WeatherWorker.h>

#include <logger/Log.h>
#include <ipc/IIpc.h>

#include <sstream>
#include <random>
#include <chrono>
#include <thread>

namespace mw { namespace proc_managers { namespace workers {

using namespace mw::ipc;

WeatherWorker::WeatherWorker(IIpc& ipcData) :
    Worker{false},
    ipcData{ipcData}
{}

void WeatherWorker::startWorking() {
    if (isWorking()) {
        INFO("Worker has already started");
        return;
    }

    ipcData.open();
    setWorkingState(true);
}

void WeatherWorker::processData() {
    if (ipcData.read() == "exit") {
        stopWorking();
    }

    if (!isWorking()) {
        INFO("Worker has not started yet");
        return;
    }

    // simulate weather data
    std::mt19937_64 mt{std::random_device{}()};
    std::uniform_real_distribution<double> temperature{-50.0, 90.0};
    std::uniform_real_distribution<double> pressure{940.0, 1060.0};
    std::uniform_int_distribution<unsigned> delta{500, 1000};
    // end simulation

    data.setTemperature(temperature(mt));
    data.setPressure(pressure(mt));

    const std::string msg = data.serialize();
    DEBUG(msg);
    ipcData.write(data.serialize());

    // // TODO: simulate waiting for sensors data...
    std::this_thread::sleep_for(std::chrono::milliseconds(delta(mt)));
    // TODO:
}

void WeatherWorker::stopWorking() {
    DEBUG("");
    if (!isWorking()) {
        INFO("Worker has already stopped");
        return;
    }

    ipcData.close();
    setWorkingState(false);
}

} } } // mw::proc_managers::workers