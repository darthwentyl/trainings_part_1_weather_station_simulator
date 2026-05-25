#pragma once

#include <proc_managers/workers/Worker.h>

namespace mw { namespace ipc {

    class IIpc;

} } // mw::ipc

namespace mw { namespace proc_managers { namespace workers {

class UserCmdWorker : public Worker {
public:
    UserCmdWorker(ipc::IIpc& ipcMemory, ipc::IIpc& ipcSocket);
    ~UserCmdWorker() = default;

    UserCmdWorker(const UserCmdWorker&) = delete;
    UserCmdWorker& operator = (const UserCmdWorker&) = delete;
    UserCmdWorker(UserCmdWorker&&) = delete;
    UserCmdWorker& operator = (UserCmdWorker&&) = delete;

    void startWorking() override;
    void processData() override;
    void stopWorking() override;
    bool isWorking() const override;

private:
    ipc::IIpc& ipcSocket;
    bool socketWorking;
};

} } } // mw::proc_managers::workers