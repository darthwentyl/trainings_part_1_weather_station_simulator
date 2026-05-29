#pragma once

#include <string>
#include <atomic>

namespace mw { namespace helpers {

class ClientTelnetHelper {
public:
    ClientTelnetHelper(const int port);
    ~ClientTelnetHelper();

    void acceptConnection(const int listenFd);
    void closeConnection();
    bool isConnected() const;
    std::string readData() const;
    bool writeData(const std::string& msg) const;

private:
    void cancelAccept();

    int connectFd;
    int port;
    std::atomic_bool shouldAcceptCancel;
};

} } // mw::helpers