#include <proc_managers/workers/Worker.h>
#include <ipc/IIpc.h>
#include <logger/Log.h>

namespace mw { namespace proc_managers { namespace workers {

using namespace mw::ipc;

Worker::Worker(const bool working, IIpc& ipcData) :
    working{working},
    ipcData{ipcData}
{}

void Worker::startWorking() {
    if (isWorking()) {
        INFO("Worker has already started");
        return;
    }
    ipcData.open();
    setWorkingState(true);
}

void Worker::stopWorking() {
    DEBUG("");
    if (!isWorking()) {
        INFO("Worker has already stopped");
        return;
    }
    ipcData.close();
    setWorkingState(false);
}

bool Worker::isWorking() {
    return  working;
}

void Worker::setWorkingState(const bool working) {
    this->working = working;
}

IIpc& Worker::ipc() {
    return ipcData;
}

} } } // mw::proc_managers::workers