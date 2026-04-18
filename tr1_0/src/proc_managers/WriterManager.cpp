#include <proc_managers/WriterManager.h>
#include <proc_managers/workers/IWorker.h>

#include <ipc/IIpc.h>
#include <ipc/ISemaphoreIpc.h>

namespace mw { namespace proc_managers {

using namespace mw::ipc;
using namespace mw::proc_managers::workers;

WriterManager::WriterManager(
    const size_t readersNum,
    ISemaphoreIpc* dataLocker,
    ISemaphoreIpc* readerLocker,
    IWorker* worker
) :
    readersNum{readersNum},
    dataLocker{dataLocker},
    readerLocker{readerLocker},
    worker{worker}
{}

void WriterManager::loop() {

}

} } // mw::proc_managers