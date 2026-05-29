#pragma once

#include <ipc/IIpc.h>
#include <helpers/ClientTelnetHelper.h>

#include <arpa/inet.h>


namespace mw { namespace ipc {

class TelnetIpc : public IIpc {
public:
    TelnetIpc(const TelnetIpc&) = delete;
    TelnetIpc& operator = (const TelnetIpc&) = delete;
    TelnetIpc(TelnetIpc&&) = delete;
    TelnetIpc& operator = (TelnetIpc&&) = delete;

    TelnetIpc(const int port);
    ~TelnetIpc();

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
    mutable helpers::ClientTelnetHelper clientConn;
};

} } // mw::ipc