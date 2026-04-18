#pragma once

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

    void startWorking() override;
    void processData() override;
    void stopWorking() override;

private:
    ipc::IIpc& ipcData;
};

} } } // mw::proc_managers::workers