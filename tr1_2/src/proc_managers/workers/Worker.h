#pragma once

#include <proc_managers/workers/IWorker.h>

namespace mw { namespace ipc {

    class IIpc;

} } // mw::ipc

namespace mw { namespace proc_managers { namespace workers {

class Worker : public  IWorker {
public:
    Worker(ipc::IIpc& ipcData);
    ~Worker() = default;

    Worker(const Worker&) = delete;
    Worker& operator = (const Worker&) = delete;
    Worker(Worker&&) = delete;
    Worker& operator = (Worker&&) = delete;

    void startWorking() override;
    void stopWorking() override;
    bool isWorking() override;

protected:
    void setWorkingState(const bool working);
    ipc::IIpc& ipc();

private:
    bool working;
    ipc::IIpc& ipcData;
};

} } }  // mw::proc_managers::workes