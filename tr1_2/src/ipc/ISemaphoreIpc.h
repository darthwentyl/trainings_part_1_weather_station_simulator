#pragma once

#include <limits>
#include <cstddef>

namespace mw { namespace ipc {

class ISemaphoreIpc {
public:
    virtual ~ISemaphoreIpc() = default;

    virtual void open(const std::size_t initial_val = std::numeric_limits<std::size_t>::max()) = 0;
    virtual void wait() = 0;
    virtual void post() = 0;
    virtual void close() = 0;
};

} } // mw::ipc