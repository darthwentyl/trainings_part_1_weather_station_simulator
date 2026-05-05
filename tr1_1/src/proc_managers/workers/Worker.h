#pragma once

#include <proc_managers/workers/IWorker.h>

namespace mw { namespace ipc {

    class IIpc;

} } // mw::ipc

namespace mw { namespace proc_managers { namespace workers {

class Worker : public  IWorker {
public:
    Worker(ipc::IIpc& ipcMemory);
    ~Worker() = default;

    Worker(const Worker&) = delete;
    Worker& operator = (const Worker&) = delete;
    Worker(Worker&&) = delete;
    Worker& operator = (Worker&&) = delete;

    void startWorking() override;
    void stopWorking() override;
    bool isWorking() const override;

protected:
    void setWorkingState(const bool working);
    ipc::IIpc& ipcMem();

private:
    bool working;
    ipc::IIpc& ipcMemory;
};

} } }  // mw::proc_managers::workes
