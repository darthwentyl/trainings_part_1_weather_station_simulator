#pragma once

#include <proc_managers/IProcManager.h>

namespace mw { namespace ipc {

    class ISemaphoreIpc;
    class IIpc;

} } // mw::ipc

namespace mw { namespace proc_managers { namespace workers {

    class IWorker;

} } } // mw::proc_managers::workers

namespace mw { namespace proc_managers {

class ReaderManager : public IProcManager {
public:
    ReaderManager(ipc::ISemaphoreIpc& dataLocker,
        ipc::ISemaphoreIpc& readerLocker,
        workers::IWorker& worker
    );
    ~ReaderManager() = default;

    ReaderManager(const ReaderManager&) = delete;
    ReaderManager& operator = (const ReaderManager&) = delete;
    ReaderManager(ReaderManager&&) = delete;
    ReaderManager& operator = (ReaderManager&&) = delete;

    void loop() override;

private:
    void start();
    void error_stop();

    ipc::ISemaphoreIpc& dataLocker;
    ipc::ISemaphoreIpc& readerLocker;
    workers::IWorker& worker;
};

} } // mw::proc_managers