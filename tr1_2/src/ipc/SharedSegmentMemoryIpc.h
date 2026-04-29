#pragma once

#include <ipc/IIpc.h>
#include <ipc/EUsageShmSegment.h>

namespace mw { namespace ipc {

class SharedSegmentMemoryIpc : public IIpc {
public:
    SharedSegmentMemoryIpc(const SharedSegmentMemoryIpc&) = delete;
    SharedSegmentMemoryIpc& operator = (const SharedSegmentMemoryIpc&) = delete;
    SharedSegmentMemoryIpc(SharedSegmentMemoryIpc&&) = delete;
    SharedSegmentMemoryIpc& operator = (SharedSegmentMemoryIpc&&) = delete;

    SharedSegmentMemoryIpc(const std::string& name, const std::size_t size, const EUsageShmSegment usage);
    ~SharedSegmentMemoryIpc();

    void open() override;
    void close() override;
    std::string read() const override;
    bool write(const std::string& msg) override;

private:
    void setDefault();

    std::string name;
    std::size_t size;
    EUsageShmSegment usage;
    int shmId;
    char* shmMem;
};

} } // mw::ipc