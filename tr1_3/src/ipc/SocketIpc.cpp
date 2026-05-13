#include <ipc/SocketIpc.h>
#include <logger/Log.h>

#include <cstring>

namespace mw { namespace ipc {

SocketIpc::SocketIpc()
    : listenFd{-1}
    , connectFd{-1}
{
    std::memset(&addr, 0, sizeof(addr));
}

SocketIpc::~SocketIpc() {

}

void SocketIpc::open() {

}

void SocketIpc::close() {

}

std::string SocketIpc::read() const {
    return std::string{};
}

bool SocketIpc::write(const std::string& msg) const {
    DEBUG(msg);
    return false;
}

} } // mw::ipc