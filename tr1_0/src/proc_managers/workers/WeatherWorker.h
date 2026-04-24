#pragma once

#include <proc_managers/workers/WeatherData.h>
#include <proc_managers/workers/Worker.h>

namespace mw { namespace ipc {

    class IIpc;

} } // mw::ipc

namespace mw { namespace proc_managers { namespace workers {

class WeatherWorker : public Worker {
public:
    WeatherWorker(ipc::IIpc& ipcData);
    ~WeatherWorker() = default;

    WeatherWorker(const WeatherWorker&) = delete;
    WeatherWorker& operator = (const WeatherWorker&) = delete;
    WeatherWorker(WeatherWorker&&) = delete;
    WeatherWorker& operator = (WeatherWorker&&) = delete;

    void processData() override;

private:
    WeatherData data;
};

} } } // mw::proc_managers::workers