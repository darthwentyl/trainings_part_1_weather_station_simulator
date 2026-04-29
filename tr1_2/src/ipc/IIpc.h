#pragma once

#include <string>

namespace mw { namespace ipc {

class IIpc {
public:
    virtual ~IIpc() = default;

    virtual void open() = 0;
    virtual void close() = 0;
    virtual std::string read() const = 0;
    virtual bool write(const std::string& msg) = 0;
};

} } // mw::ipc