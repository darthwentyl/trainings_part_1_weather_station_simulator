#pragma once

#include <ipc/IIpc.h>

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
    int listenFd;
    int connectFd;
    int port;
    int opt;
    struct sockaddr_in addr;
};

} } // mw::ipc