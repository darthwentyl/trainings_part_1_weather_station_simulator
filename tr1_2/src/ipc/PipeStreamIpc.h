#pragma once

#include <ipc/IIpc.h>
#include <ipc/EPipeMode.h>

namespace mw { namespace ipc {

class PipeStreamIpc : public IIpc {
public:
    PipeStreamIpc(const PipeStreamIpc&) = delete;
    PipeStreamIpc& operator = (const PipeStreamIpc&) = delete;
    PipeStreamIpc(PipeStreamIpc&&) = delete;
    PipeStreamIpc& operator = (PipeStreamIpc&&) = delete;

    PipeStreamIpc(const std::string& command, const EPipeMode mode);
    ~PipeStreamIpc();

    void open() override;
    void close() override;
    std::string read() const override;
    bool write(const std::string& msg) override;

private:
    std::string getMode() const;

    std::string command;
    EPipeMode mode;
    FILE* stream;
};

} } // mw::ipc