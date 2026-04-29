#include <proc_managers/ReaderManager.h>
#include <proc_managers/workers/IWorker.h>

#include <logger/Log.h>

#include <ipc/IIpc.h>
#include <ipc/ISemaphoreIpc.h>

namespace mw { namespace proc_managers {

using namespace mw::ipc;
using namespace mw::proc_managers::workers;

ReaderManager::ReaderManager(ISemaphoreIpc& dataLocker,
    ISemaphoreIpc& readerLocker,
    IWorker& worker
) :
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
        error_stop();
    }
}

void ReaderManager::start() {
    readerLocker.open();
    dataLocker.open();
    worker.startWorking();
}

void ReaderManager::error_stop() {
    worker.stopWorking();
    readerLocker.close();
    dataLocker.close();
}

} } // mw::proc_managers