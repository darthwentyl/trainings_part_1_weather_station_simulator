#include <proc_managers/ReaderManager.h>
#include <proc_managers/workers/IWorker.h>

#include <ipc/IIpc.h>
#include <ipc/ISemaphoreIpc.h>

namespace mw { namespace proc_managers {

using namespace mw::ipc;
using namespace mw::proc_managers::workers;

ReaderManager::ReaderManager(ISemaphoreIpc* dataLocker,
    ISemaphoreIpc* readerLocker,
    IWorker* worker
) :
    dataLocker{dataLocker},
    readerLocker{readerLocker},
    worker{worker}
{}

void ReaderManager::loop() {

}

} } // mw::proc_managers