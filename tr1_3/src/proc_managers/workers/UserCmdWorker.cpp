#include <proc_managers/workers/UserCmdWorker.h>

#include <logger/Log.h>
#include <ipc/IIpc.h>

namespace mw { namespace proc_managers { namespace workers {

using namespace mw::ipc;

UserCmdWorker::UserCmdWorker(IIpc& ipcMemory, IIpc& ipcSocket) :
    Worker{ipcMemory},
    ipcSocket{ipcSocket},
    socketWorking{false}
{}

void UserCmdWorker::startWorking() {
    Worker::startWorking();

    if (socketWorking) {
        INFO("Socket has already started");
        return;
    }
    ipcSocket.open();
    socketWorking = true;
}

void UserCmdWorker::processData() {
    if (!isWorking()) {
        INFO("Worker has not started yet");
        return;
    }
}

void UserCmdWorker::stopWorking() {
    Worker::stopWorking();

    if (!socketWorking) {
        INFO("Socket has already stopped");
        return;
    }
    ipcSocket.close();
    socketWorking = false;
}

bool UserCmdWorker::isWorking() const {
    return Worker::isWorking() && socketWorking;
}

} } } // mw::proc_managers::worker