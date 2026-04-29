#pragma once

#include <ipc/ISemaphoreIpc.h>
#include <ipc/EUsageShmSegment.h>

#include <string>

namespace mw { namespace ipc {

class SharedSegmentSemaphoreIpc : public ISemaphoreIpc {
public:
    SharedSegmentSemaphoreIpc(const std::string& name, const EUsageShmSegment usage);
    ~SharedSegmentSemaphoreIpc();

    SharedSegmentSemaphoreIpc(const SharedSegmentSemaphoreIpc&) = delete;
    SharedSegmentSemaphoreIpc& operator = (const SharedSegmentSemaphoreIpc&) = delete;
    SharedSegmentSemaphoreIpc(SharedSegmentSemaphoreIpc&&) = delete;
    SharedSegmentSemaphoreIpc& operator = (SharedSegmentSemaphoreIpc&&) = delete;

    void open(const std::size_t initial_val = std::numeric_limits<std::size_t>::max()) override;
    void wait() override;
    void post() override;
    void close() override;

private:
    void setDefault();

    EUsageShmSegment usage;
    std::string semName;
    int semId;
};

} } // mw::ipc