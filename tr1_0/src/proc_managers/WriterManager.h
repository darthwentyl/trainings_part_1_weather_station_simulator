#pragma once

#include <cstddef>

#include <proc_managers/IProcManager.h>

namespace mw { namespace ipc {

    class ISemaphoreIpc;
    class IIpc;

} } // mw::ipc

namespace mw { namespace proc_managers { namespace workers {

    class IWorker;

} } } // mw::proc_managers::workers

namespace mw { namespace proc_managers {

class WriterManager : public IProcManager {
public:
    WriterManager(const std::size_t readersNum,
        ipc::ISemaphoreIpc& dataLocker,
        ipc::ISemaphoreIpc& readerLocker,
        workers::IWorker& worker
    );
    ~WriterManager() = default;

    WriterManager(const WriterManager&) = delete;
    WriterManager& operator = (const WriterManager&) = delete;
    WriterManager(WriterManager&&) = delete;
    WriterManager& operator = (WriterManager&&) = delete;

    void loop() override;

private:
    const size_t readersNum;
    ipc::ISemaphoreIpc& dataLocker;
    ipc::ISemaphoreIpc& readerLocker;
    workers::IWorker& worker;
};

} } // mw::proc_managers