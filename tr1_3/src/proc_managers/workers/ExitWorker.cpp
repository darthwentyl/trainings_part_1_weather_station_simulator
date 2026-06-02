#include <proc_managers/workers/ExitWorker.h>

#include <logger/Log.h>
#include <ipc/IIpc.h>

namespace mw { namespace proc_managers { namespace workers {

using namespace mw::ipc;

ExitWorker::ExitWorker(IIpc& ipcMemory) :
    Worker{ipcMemory}
{}

void ExitWorker::processData() {
    DEBUG("begin");
    if (!isWorking()) {
        INFO("Worker has not started yet");
        return;
    }

    if (ipcMem().read() == "exit") {
        return stopWorking();
    }

    ipcMem().write("exit");
    DEBUG("end");
}

} } } // mw::proc_managers::workers