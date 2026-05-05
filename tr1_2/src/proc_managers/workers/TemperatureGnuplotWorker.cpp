#include <proc_managers/workers/TemperatureGnuplotWorker.h>

#include <logger/Log.h>
#include <ipc/IIpc.h>

namespace mw { namespace proc_managers { namespace workers {

using namespace mw::ipc;

TemperatureGnuplotWorker::TemperatureGnuplotWorker(IIpc& ipcData, IIpc& pipeIpc) :
    Worker{ipcData},
    pipeIpc{pipeIpc}
{}



void TemperatureGnuplotWorker::processData() {
    if (!isWorking()) {
        INFO("Worker has not started yet");
        return;
    }

    if (ipc().read() == "exit") {
        return stopWorking();
    }
}

} } } // mw::proc_managers::workers