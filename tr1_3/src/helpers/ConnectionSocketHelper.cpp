#include <helpers/ConnectionSocketHelper.h>
#include <logger/Log.h>
#include <exceptions/socket_error.h>

#include <cstring>
#include <sys/socket.h>
#include <unistd.h>


namespace mw { namespace helpers {

using namespace mw::exceptions;

constexpr const int FAILURE = -1;
constexpr const int NOT_CONNECTED = -2;
constexpr const std::size_t BUFF_SIZE = 128;
constexpr const std::size_t TELNET_EMPTY_MSG_SIZE = 2;

ConnectionSocketHelper::ConnectionSocketHelper()
    : connectFd{NOT_CONNECTED}
{}

ConnectionSocketHelper::~ConnectionSocketHelper() {

    try {
        closeConnection();
    } catch (const socket_error& e) {
        ERROR(e.what());
    }
}

void ConnectionSocketHelper::acceptConnection(const int listenFd) {
    connectFd = accept(listenFd, nullptr, nullptr);
    if (connectFd == FAILURE) {
        throw socket_error{__FUNCTION__, __LINE__, std::string{"accept failed "} + std::string{strerror(errno)}};
    }
}

void ConnectionSocketHelper::closeConnection() {
    if (isConnected()) {
        if (close(connectFd) == FAILURE) {
            connectFd = FAILURE;
            throw socket_error{__FUNCTION__, __LINE__, std::string{"close failed "} + std::string{strerror(errno)}};
        }
        connectFd = NOT_CONNECTED;
    }
}

bool ConnectionSocketHelper::isConnected() const {
    return connectFd != NOT_CONNECTED && connectFd != FAILURE;
}

std::string ConnectionSocketHelper::readData() const {
    if (!isConnected()) {
        throw socket_error{__FUNCTION__, __LINE__, "no user connected"};
    }

    std::string data{};
    char buff[BUFF_SIZE];
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

} } // mw