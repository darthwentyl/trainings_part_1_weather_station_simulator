#include <proc_managers/workers/ExitWorker.h>

#include <logger/Log.h>
#include <ipc/IIpc.h>

namespace mw { namespace proc_managers { namespace workers {

using namespace mw::ipc;

ExitWorker::ExitWorker(IIpc& ipcData) :
    Worker{false},
    ipcData{ipcData}
{}

void ExitWorker::startWorking() {
    if (isWorking()) {
        INFO("Worker has already started");
        return;
    }

    ipcData.open();

    setWorkingState(true);
}

void ExitWorker::processData() {
    if (!isWorking()) {
        INFO("Worker has not started yet");
        return;
    }

    if (ipcData.read() == "exit") {
        return stopWorking();
    }

    ipcData.write("exit");
}

void ExitWorker::stopWorking() {
    if (!isWorking()) {
        INFO("Worker has already stopped");
        return;
    }

    ipcData.close();
    setWorkingState(false);
}

} } } // mw::proc_managers::workers