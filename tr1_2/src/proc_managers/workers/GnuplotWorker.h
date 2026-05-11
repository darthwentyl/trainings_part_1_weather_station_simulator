#pragma once

#include <proc_managers/workers/Worker.h>
#include <helpers/GnuplotDescription.h>
#include <helpers/GnuplotCommandExecutor.h>

namespace mw { namespace ipc {

    class IIpc;

} } // mw::ipc

namespace mw { namespace proc_managers { namespace workers {

class GnuplotWorker : public Worker {
public:
    GnuplotWorker(ipc::IIpc& ipcMemory, ipc::IIpc& ipcPipe, const helpers::GnuplotDescription& gnuplotDescription);
    ~GnuplotWorker() = default;

    GnuplotWorker(const GnuplotWorker&) = delete;
    GnuplotWorker& operator = (const GnuplotWorker&) = delete;
    GnuplotWorker(GnuplotWorker&&) = delete;
    GnuplotWorker& operator = (GnuplotWorker&&) = delete;

    void startWorking() override;
    void processData() override;
    void stopWorking() override;
    bool isWorking() const override;

private:
    void configureGnuplot();

    ipc::IIpc& ipcPipe;
    bool pipeWorking;
    helpers::GnuplotCommandExecutor executor;

};

} } } // mw::proc_managers::workers