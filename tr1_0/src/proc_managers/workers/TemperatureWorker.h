#pragma once

#include <proc_managers/workers/Worker.h>

namespace mw { namespace ipc {

    class IIpc;

} } // mw::ipc

namespace mw { namespace proc_managers { namespace workers {

class TemperatureWorker : public Worker {
public:
    TemperatureWorker(ipc::IIpc& ipcData, const std::size_t bufferSize);
    ~TemperatureWorker() = default;

    TemperatureWorker(const TemperatureWorker&) = delete;
    TemperatureWorker& operator = (const TemperatureWorker&) = delete;
    TemperatureWorker(TemperatureWorker&&) = delete;
    TemperatureWorker& operator = (TemperatureWorker&&) = delete;

    void startWorking() override;
    void processData() override;
    void stopWorking() override;

private:
    ipc::IIpc& ipcData;
    const std::size_t bufferSize;
};

} } } // mw::proc_managers::workers