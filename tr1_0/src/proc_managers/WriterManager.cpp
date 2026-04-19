#include <proc_managers/WriterManager.h>
#include <proc_managers/workers/IWorker.h>

#include <logger/Log.h>

#include <ipc/IIpc.h>
#include <ipc/ISemaphoreIpc.h>

namespace mw { namespace proc_managers {

using namespace mw::ipc;
using namespace mw::proc_managers::workers;

WriterManager::WriterManager(
    const std::size_t readersNum,
    ISemaphoreIpc& dataLocker,
    ISemaphoreIpc& readerLocker,
    IWorker& worker
) :
    readersNum{readersNum},
    dataLocker{dataLocker},
    readerLocker{readerLocker},
    worker{worker}
{}

void WriterManager::loop() {
    try{
        worker.startWorking();

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
        worker.stopWorking();
    }
}

} } // mw::proc_managers