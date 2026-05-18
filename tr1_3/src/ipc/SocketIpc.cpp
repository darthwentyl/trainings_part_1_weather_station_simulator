#include <ipc/SocketIpc.h>
#include <logger/Log.h>

#include <exceptions/socket_error.h>

#include <cstring>

#include <sys/socket.h>
#include <unistd.h>

namespace mw { namespace ipc {

using namespace mw::exceptions;

constexpr const int FAILURE = -1;
constexpr const int NOT_CREATED = -2;
constexpr const int BACKLOG = 1;
constexpr const std::size_t BUFF_SIZE = 128;

SocketIpc::SocketIpc(const int port)
    : listenFd{NOT_CREATED}
    , connectFd{NOT_CREATED}
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

    if (!isConnectionOpened()) {
        acceptConnection();
    }
}

void SocketIpc::close() {

}

std::string SocketIpc::readData() const {
    if (!isConnectionOpened()) {
        throw socket_error{__FUNCTION__, __LINE__, "connection has not accepted. You cannot read data."};
    }

    std::string msg;
    char buff[BUFF_SIZE];
    std::size_t n = 0;

    while ((n = read(connectFd, buff, sizeof(buff))) > 0) {

    }

    return std::string{};
}

bool SocketIpc::writeData(const std::string& msg) const {
    if (!isConnectionOpened()) {
        throw socket_error{__FUNCTION__, __LINE__, "connection has not accepted. You cannot write data"};
    }

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

void SocketIpc::acceptConnection() {
    connectFd = accept(listenFd, nullptr, nullptr);
    if (connectFd == FAILURE) {
        throw socket_error{__FUNCTION__, __LINE__, "accept failed " + std::string{strerror(errno)}};
    }
}

bool SocketIpc::isConnectionOpened() const {
    return connectFd != NOT_CREATED && connectFd != FAILURE;
}

bool SocketIpc::isSocketOpened() const {
    return listenFd != NOT_CREATED && listenFd != FAILURE;
}

} } // mw::ipc