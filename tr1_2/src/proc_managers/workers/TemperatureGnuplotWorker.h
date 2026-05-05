#pragma once

#include <proc_managers/workers/Worker.h>

namespace mw { namespace ipc {

    class IIpc;

} } // mw::ipc

namespace mw { namespace proc_managers { namespace workers {

class TemperatureGnuplotWorker : public Worker {
    TemperatureGnuplotWorker(ipc::IIpc& ipcData, ipc::IIpc& pipeIpc);
    ~TemperatureGnuplotWorker() = default;

    TemperatureGnuplotWorker(const TemperatureGnuplotWorker&) = delete;
    TemperatureGnuplotWorker& operator = (const TemperatureGnuplotWorker&) = delete;
    TemperatureGnuplotWorker(TemperatureGnuplotWorker&&) = delete;
    TemperatureGnuplotWorker& operator = (TemperatureWorker&&) = delete;

    void startWorking() override;
    void processData() override;
    void stopWorking() override;

private:
    ipc::IIpc& pipeIpc;
};

} } } // mw::proc_managers::workers