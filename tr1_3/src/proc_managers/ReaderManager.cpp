#include <proc_managers/ReaderManager.h>
#include <proc_managers/workers/IWorker.h>

#include <logger/Log.h>

#include <ipc/IIpc.h>
#include <ipc/ISemaphoreIpc.h>

#include <signal.h>

namespace mw { namespace proc_managers {

using namespace mw::ipc;
using namespace mw::proc_managers::workers;

ReaderManager::ReaderManager(const pid_t parentPid, ISemaphoreIpc& dataLocker,
    ISemaphoreIpc& readerLocker,
    IWorker& worker
) :
    parentPid{parentPid},
    dataLocker{dataLocker},
    readerLocker{readerLocker},
    worker{worker}
{}

void ReaderManager::loop() {
    try {
        start();
        while (worker.isWorking()) {
            readerLocker.wait();
            dataLocker.wait();
            worker.processData();
            dataLocker.post();
        }
    } catch (const std::exception& e) {
        ERROR(e.what());
        kill(parentPid, SIGINT);
    }
}

void ReaderManager::start() {
    readerLocker.open();
    dataLocker.open();
    worker.startWorking();
}

} } // mw::proc_managers