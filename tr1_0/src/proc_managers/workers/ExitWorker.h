#pragma once

#include <proc_managers/workers/Worker.h>

namespace mw { namespace ipc {

    class IIpc;

} } // mw::ipc

namespace mw { namespace proc_managers { namespace workers {

class ExitWorker : public Worker {
public:
    ExitWorker(ipc::IIpc& ipcData);
    ~ExitWorker() = default;

    ExitWorker(const ExitWorker&) = delete;
    ExitWorker& operator = (const ExitWorker&) = delete;
    ExitWorker(ExitWorker&&) = delete;
    ExitWorker& operator = (ExitWorker&&) = delete;

    void startWorking() override;
    void processData() override;
    void stopWorking() override;

private:
    ipc::IIpc& ipcData;
};

} } } // mw::proc_managers::workers