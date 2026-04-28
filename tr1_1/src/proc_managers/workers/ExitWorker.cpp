#include <proc_managers/workers/ExitWorker.h>

#include <logger/Log.h>
#include <ipc/IIpc.h>

namespace mw { namespace proc_managers { namespace workers {

using namespace mw::ipc;

ExitWorker::ExitWorker(IIpc& ipcData) :
    Worker{false, ipcData}
{}

void ExitWorker::processData() {
    if (!isWorking()) {
        INFO("Worker has not started yet");
        return;
    }

    if (ipc().read() == "exit") {
        return stopWorking();
    }

    ipc().write("exit");
}

} } } // mw::proc_managers::workers