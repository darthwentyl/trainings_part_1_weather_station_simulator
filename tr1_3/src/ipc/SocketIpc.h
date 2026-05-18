#pragma once

#include <ipc/IIpc.h>
#include <helpers/ConnectionSocketHelper.h>

#include <arpa/inet.h>


namespace mw { namespace ipc {

class SocketIpc : public IIpc {
public:
    SocketIpc(const SocketIpc&) = delete;
    SocketIpc& operator = (const SocketIpc&) = delete;
    SocketIpc(SocketIpc&&) = delete;
    SocketIpc& operator = (SocketIpc&&) = delete;

    SocketIpc(const int port);
    ~SocketIpc();

    void open() override;
    void close() override;
    std::string read() const override;
    bool write(const std::string& msg) const override;

private:
    void openSocket();
    bool isSocketOpened() const;

    int listenFd;
    int port;
    int opt;
    struct sockaddr_in addr;
    mutable helpers::ConnectionSocketHelper clientConn;
};

} } // mw::ipc