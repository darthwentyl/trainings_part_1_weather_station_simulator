#pragma once

#include <proc_managers/workers/IWorker.h>

namespace mw { namespace ipc {

    class IIpc;

} } // mw::ipc

namespace mw { namespace proc_managers { namespace workers {

class Worker : public  IWorker {
public:
    Worker(const bool working, ipc::IIpc& ipcData);
    ~Worker() = default;

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