#include <ipc/PipeStreamIpc.h>
#include <logger/Log.h>

#include <exceptions/pipe_error.h>

namespace mw { namespace ipc {

using namespace mw::exceptions;

PipeStreamIpc::PipeStreamIpc(const std::string& command, const EPipeMode mode)
    : command{command}
    , mode{mode}
    , stream{nullptr}
{}

PipeStreamIpc::~PipeStreamIpc() {

}

void PipeStreamIpc::open() {
    if (stream != nullptr) {
        INFO("Pipe stream for command " << command << " has already opened");
        return;
    }

    if (command.empty()) {
        throw pipe_error{__FUNCTION__, __LINE__, "Command is empty"};
    }

    stream = popen(command.c_str(), getMode().c_str());

    if (stream == nullptr) {
        throw pipe_error{__FUNCTION__, __LINE__, "Cannot open pipe stream to command " + command};
    }
}

void PipeStreamIpc::close() {

}

std::string PipeStreamIpc::read() const {
    return std::string{};
}

bool PipeStreamIpc::write(const std::string& msg) {
    return false;
}

std::string PipeStreamIpc::getMode() const {
    switch (mode) {
    case EPipeMode::READ: return "r";
    case EPipeMode::WRITE:
    default:
        return "w";
    }
}

} } // mw::ipc