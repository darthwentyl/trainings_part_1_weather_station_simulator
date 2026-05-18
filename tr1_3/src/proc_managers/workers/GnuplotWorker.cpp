#include <proc_managers/workers/GnuplotWorker.h>
#include <helpers/GnuplotCommander.h>

#include <logger/Log.h>
#include <ipc/IIpc.h>

namespace mw { namespace proc_managers { namespace workers {

using namespace mw::ipc;
using namespace mw::helpers;

GnuplotWorker::GnuplotWorker(IIpc& ipcMemory, IIpc& ipcPipe, const GnuplotDescription& gnuplotDescription) :
    Worker{ipcMemory},
    ipcPipe{ipcPipe},
    pipeWorking{false},
    executor{ipcPipe, gnuplotDescription}
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

    if (ipcMem().readData() == "exit") {
        return stopWorking();
    }

    executor.execute(EGnuplotCommand::PLOT_POINTS);
}

void GnuplotWorker::stopWorking() {
    Worker::stopWorking();

    if (!pipeWorking) {
        INFO("Pipe has already stopped");
        return;
    }
    ipcPipe.close();
    pipeWorking = false;
}

bool GnuplotWorker::isWorking() const {
    return Worker::isWorking() && pipeWorking;
}

void GnuplotWorker::configureGnuplot() {
    executor.execute(EGnuplotCommand::TERMINAL);
    executor.execute(EGnuplotCommand::TITLE);
    executor.execute(EGnuplotCommand::AXIS_LABELS);
    executor.execute(EGnuplotCommand::GRID);
}


} } } // mw::proc_managers::workers