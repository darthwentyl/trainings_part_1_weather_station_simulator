#include <ipc/SocketIpc.h>
#include <logger/Log.h>

#include <exceptions/socket_error.h>

#include <cstring>
#include <sys/socket.h>
#include <unistd.h>

namespace mw { namespace ipc {

using namespace mw::exceptions;
using namespace mw::helpers;

namespace {

constexpr const int FAILURE = -1;
constexpr const int NOT_CREATED = -2;
constexpr const int BACKLOG = 1;

void closeSocket(int& listenFd) {
    close(listenFd);
}

} // anonymous



SocketIpc::SocketIpc(const int port)
    : listenFd{NOT_CREATED}
    , port{port}
    , opt{1}
{
    std::memset(&addr, 0, sizeof(addr));
}

SocketIpc::~SocketIpc() {

}

void SocketIpc::open() {
    if (!isSocketOpened()) {
        openSocket();
    }
}

void SocketIpc::close() {
    closeSocket(listenFd);
}

std::string SocketIpc::read() const {
    if (!isSocketOpened()) {
        throw socket_error{__FUNCTION__, __LINE__, "socket has not opened. Cannot read data"};
    }

    if (!clientConn.isConnected()) {
        clientConn.acceptConnection(listenFd);
    }

    std::string data = clientConn.readData();
    if (data.size() == 0) {
        clientConn.closeConnection();
    }

    return data;
}

bool SocketIpc::write(const std::string& msg) const {
    DEBUG(msg);
    return false;
}

void SocketIpc::openSocket() {
    listenFd = socket(AF_INET, SOCK_STREAM, 0);
    if (listenFd == FAILURE) {
        throw socket_error{__FUNCTION__, __LINE__, "creation socket failed " + std::string{strerror(errno)}};
    }

    if (setsockopt(listenFd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) == FAILURE) {
        throw socket_error{__FUNCTION__, __LINE__, "setsockopt failed " + std::string{strerror(errno)}};
    }

    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_port = htons(port);

    if (bind(listenFd, (struct sockaddr*)&addr, sizeof(addr)) == FAILURE) {
        throw socket_error{__FUNCTION__, __LINE__, "bind failed " + std::string{strerror(errno)}};
    }

    if (listen(listenFd, BACKLOG) == FAILURE) {
        throw socket_error{__FUNCTION__, __LINE__, "listen failed " + std::string{strerror(errno)}};
    }

    INFO("Server listening on port " << port);
}

bool SocketIpc::isSocketOpened() const {
    return listenFd != NOT_CREATED && listenFd != FAILURE;
}

} } // mw::ipc