#pragma once

#include <string>

namespace mw { namespace helpers {

class ConnectionSocketHelper {
public:
    ConnectionSocketHelper();
    ~ConnectionSocketHelper();

    void acceptConnection(const int listenFd);
    void closeConnection();
    bool isConnected() const;
    std::string readData() const;
    bool writeData(const std::string& msg) const;


private:
    int connectFd;
};

} } // mw::helpers