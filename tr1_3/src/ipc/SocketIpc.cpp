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
    if (close(listenFd) == FAILURE) {
        listenFd = FAILURE;
        throw socket_error{__FUNCTION__, __LINE__, std::string{"close failed"} + std::string{strerror(errno)}};
    }
    listenFd = NOT_CREATED;
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
    try {
        close();
    } catch (const socket_error& e) {
        ERROR(e.what());
    }
}

void SocketIpc::open() {
    if (!isSocketOpened()) {
        openSocket();
    }
}

void SocketIpc::close() {
    if (isSocketOpened()) {
        closeSocket(listenFd);
    }
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
        return std::string{"Client disconnected"};
    }

    return data;
}

bool SocketIpc::write(const std::string& msg) const {
    DEBUG(msg);
    if (!isSocketOpened()) {
        throw socket_error{__FUNCTION__, __LINE__, "socket has not opened. Cannot write data"};
    }

    if (!clientConn.isConnected()) {
        clientConn.acceptConnection(listenFd);
    }

    return clientConn.writeData(msg);
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