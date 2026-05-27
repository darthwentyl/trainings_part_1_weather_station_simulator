#include <helpers/ConnectionSocketHelper.h>
#include <logger/Log.h>
#include <exceptions/socket_error.h>

#include <cstring>
#include <sys/socket.h>
#include <sys/select.h>
#include <unistd.h>
#include <arpa/inet.h>

namespace mw { namespace helpers {

using namespace mw::exceptions;

constexpr const int FAILURE = -1;
constexpr const int NOT_CONNECTED = -2;
constexpr const std::size_t BUFF_SIZE = 128;
constexpr const std::size_t TELNET_EMPTY_MSG_SIZE = 2;

ConnectionSocketHelper::ConnectionSocketHelper(const int port)
    : connectFd{NOT_CONNECTED}
    , port{port}
    , shouldAcceptCancel{false}
{}

ConnectionSocketHelper::~ConnectionSocketHelper() {
    try {
        closeConnection();
    } catch (const socket_error& e) {
        ERROR(e.what());
    }
}

void ConnectionSocketHelper::acceptConnection(const int listenFd) {
    shouldAcceptCancel = true;
    connectFd = accept(listenFd, nullptr, nullptr);
    if (connectFd == FAILURE) {
        shouldAcceptCancel = false;
        throw socket_error{__FUNCTION__, __LINE__, std::string{"accept failed "} + std::string{strerror(errno)}};
    }
    shouldAcceptCancel = false;
}

void ConnectionSocketHelper::closeConnection() {
    DEBUG("isConnected(): " << std::boolalpha << isConnected());
    if (isConnected()) {
        if (shutdown(connectFd, SHUT_RDWR) == FAILURE) {
            DEBUG("shutdown(" << connectFd << ", SHUT_RDWR) failed.");
        }
        if (close(connectFd) == FAILURE) {
            connectFd = FAILURE;
            throw socket_error{__FUNCTION__, __LINE__, std::string{"close failed "} + std::string{strerror(errno)}};
        }
        connectFd = NOT_CONNECTED;
        return;
    }

    DEBUG("shouldAcceptCancel: " << std::boolalpha << shouldAcceptCancel);
    if (shouldAcceptCancel) {
        cancelAccept();
    }
}

bool ConnectionSocketHelper::isConnected() const {
    return connectFd != NOT_CONNECTED && connectFd != FAILURE;
}

std::string ConnectionSocketHelper::readData() const {
    if (!isConnected()) {
        throw socket_error{__FUNCTION__, __LINE__, "no user connected"};
    }

    char buff[BUFF_SIZE];
    std::string data{};
    std::size_t n = 0;
    bool readingFinish = false;
    while (!readingFinish && (n = read(connectFd, buff, sizeof(buff))) > 0) {
        // strip \n\r
        while (n > 0 && (buff[n-1] == '\n' || buff[n-1] == '\r')) {
            readingFinish = true;
            n--;
        }
        buff[n] = '\0';
        data.append(buff);
        DEBUG("data: " << data);
    }
    return data;
}

bool ConnectionSocketHelper::writeData(const std::string& msg) const {
    if (!isConnected()) {
        throw socket_error{__FUNCTION__, __LINE__, "no user connected"};
    }

    // no send data is not critical
    write(connectFd, msg.c_str(), msg.size());

    return true;
}

void ConnectionSocketHelper::cancelAccept() {
    int clientFd = socket(AF_INET, SOCK_STREAM, 0);
    if (clientFd == FAILURE) {
        ERROR("socket(AF_INET, SOCK_STREAM, 0) failed: " << strerror(errno));
        return;
    }

    struct sockaddr_in server{};
    memset(&server, 0, sizeof(server));
    server.sin_family = AF_INET;
    server.sin_port = htons(port);
    inet_pton(AF_INET, "127.0.0.1", &server.sin_addr);

    if (connect(clientFd, reinterpret_cast<struct sockaddr*>(&server), sizeof(server)) == FAILURE) {
        ERROR("connect(clientFd, server, sizeof(server)) failed: " << strerror(errno));
        close(clientFd);
        return;
    }

    const std::string exitMsg{};
    send(clientFd, exitMsg.c_str(), exitMsg.size(), 0);
    close(clientFd);
}

} } // mw