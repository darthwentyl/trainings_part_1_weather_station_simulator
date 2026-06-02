#include <proc_managers/WriterManager.h>
#include <proc_managers/workers/IWorker.h>

#include <logger/Log.h>

#include <ipc/IIpc.h>
#include <ipc/ISemaphoreIpc.h>

#include <signal.h>

namespace mw { namespace proc_managers {

using namespace mw::ipc;
using namespace mw::proc_managers::workers;

WriterManager::WriterManager(
    const pid_t parentPid,
    const std::size_t readersNum,
    ISemaphoreIpc& dataLocker,
    ISemaphoreIpc& readerLocker,
    IWorker& worker
) :
    parentPid{parentPid},
    readersNum{readersNum},
    dataLocker{dataLocker},
    readerLocker{readerLocker},
    worker{worker}
{}

void WriterManager::loop() {
    try{
        start();
        while (worker.isWorking()) {
            dataLocker.wait();
            worker.processData();
            dataLocker.post();
            for (std::size_t reader = 0; reader < readersNum; ++reader) {
                readerLocker.post();
            }
        }
    } catch (const std::exception& e) {
        ERROR(e.what());
        kill(parentPid, SIGINT);
    }
}

void WriterManager::start() {
    DEBUG("begin");
    dataLocker.open();
    readerLocker.open();
    worker.startWorking();
    DEBUG("end");
}

} } // mw::proc_managers