#include <proc_managers/workers/GnuplotWorker.h>
#include <helpers/GnuplotCommander.h>

#include <logger/Log.h>
#include <ipc/IIpc.h>

namespace mw { namespace proc_managers { namespace workers {

using namespace mw::ipc;
using namespace mw::helpers;

GnuplotWorker::GnuplotWorker(IIpc& ipcMemory, IIpc& ipcPipe) :
    Worker{ipcMemory},
    ipcPipe{ipcPipe},
    pipeWorking{false}
{}

void GnuplotWorker::startWorking() {
    Worker::startWorking();

    if (pipeWorking) {
        INFO("Pipe has already started");
        return;
    }
    ipcPipe.open();
    pipeWorking = true;

    configureGnuplot();
}

void GnuplotWorker::processData() {
    if (!isWorking()) {
        INFO("Worker has not started yet");
        return;
    }

    if (ipcMem().read() == "exit") {
        return stopWorking();
    }


}

void GnuplotWorker::stopWorking() {

}

bool GnuplotWorker::isWorking() const {
    return Worker::isWorking() && pipeWorking;
}

void GnuplotWorker::configureGnuplot() {
    ipcPipe.write(GnuplotCommander::terminal(800, 600));
    ipcPipe.write(GnuplotCommander::title("Temperature sensors measurement"));
    ipcPipe.write(GnuplotCommander::axisLabel(EGnuplotAxis::OX, "n"));
    ipcPipe.write(GnuplotCommander::axisLabel(EGnuplotAxis::OY, "T[C]"));
    ipcPipe.write(GnuplotCommander::grid(true));
}


} } } // mw::proc_managers::workers