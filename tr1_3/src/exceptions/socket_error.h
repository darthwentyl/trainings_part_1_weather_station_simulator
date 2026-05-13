#pragma once

#include <exception>
#include <string>
#include <sstream>

namespace mw { namespace exceptions {

class socket_error : std::exception {
public:
    socket_error(const char* func, const std::size_t line, const std::string& msg) {
        std::ostringstream oss;
        oss << func << ":" << line << ": " << msg;
        message = oss.str();
    }

    ~socket_error() = default;

    const char* what() const noexcept override {
        return message.c_str();
    }

private:
    std::string message;
}

} } // mw::exceptions