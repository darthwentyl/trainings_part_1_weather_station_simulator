#include <ipc/PipeStreamIpc.h>
#include <logger/Log.h>

#include <exceptions/pipe_error.h>

#include <cstring>

namespace mw { namespace ipc {

using namespace mw::exceptions;

constexpr const int FAILURE = -1;
constexpr const std::size_t READ_BUFF_SIZE = 1024;

PipeStreamIpc::PipeStreamIpc(const std::string& command, const EPipeMode mode)
    : command{command}
    , mode{mode}
    , stream{nullptr}
{}

PipeStreamIpc::~PipeStreamIpc() {
    try {
        if (stream != nullptr) {
            close();
        }
    } catch (const std::exception& e) {
        ERROR(e.what());
    }
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
        throw pipe_error{__FUNCTION__, __LINE__, "Cannot open pipe stream for command: " + command};
    }
    DEBUG("Command " << command << " has already opened");
}

void PipeStreamIpc::close() {
    if (stream == nullptr) {
        INFO("Pipe stream for command " << command << " has already cloed");
        return;
    }

    if (pclose(stream) == FAILURE) {
        stream = nullptr;
        throw pipe_error{__FUNCTION__, __LINE__, "Cannot close pipe stream for command: " + command};
    }
    stream = nullptr;
}

std::string PipeStreamIpc::read() const {
    if (stream == nullptr) {
        throw pipe_error{__FUNCTION__, __LINE__, "Stream has not started, yet"};
    }

    if (mode == EPipeMode::WRITE) {
        throw pipe_error{__FUNCTION__, __LINE__, "Cannot read data when you are in write mode"};
    }

    char buff[READ_BUFF_SIZE];
    std::string data;
    data.reserve(READ_BUFF_SIZE);

    while(fgets(buff, READ_BUFF_SIZE, stream) != nullptr) {
        data.append(buff);
    }

    DEBUG("Received data: " << data);
    return data;
}

bool PipeStreamIpc::write(const std::string& msg) {
    if (stream == nullptr) {
        throw pipe_error{__FUNCTION__, __LINE__, "Stream has not started, yet"};
    }

    if (mode == EPipeMode::READ) {
        throw pipe_error{__FUNCTION__, __LINE__, "Cannot write data when you are in read mode"};
    }

    if (fputs(msg.c_str(), stream) == EOF) {
        throw pipe_error{__FUNCTION__, __LINE__, "Write msg to " + command + " failed"};
    }

    fflush(stream);

    return true;
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