#include <proc_managers/workers/Worker.h>
#include <ipc/IIpc.h>
#include <logger/Log.h>

namespace mw { namespace proc_managers { namespace workers {

using namespace mw::ipc;

Worker::Worker(IIpc& ipcMemory) :
    working{false},
    ipcMemory{ipcMemory}
{}

void Worker::startWorking() {
    if (isWorking()) {
        INFO("Worker has already started");
        return;
    }
    ipcMemory.open();
    setWorkingState(true);
}

void Worker::stopWorking() {
    DEBUG("");
    if (!isWorking()) {
        INFO("Worker has already stopped");
        return;
    }
    ipcMemory.close();
    setWorkingState(false);
}

bool Worker::isWorking() const {
    return  working;
}

void Worker::setWorkingState(const bool working) {
    this->working = working;
}

IIpc& Worker::ipcMem() {
    return ipcMemory;
}

} } } // mw::proc_managers::workers