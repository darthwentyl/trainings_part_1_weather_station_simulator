#pragma once

#include <exception>
#include <string>
#include <sstream>

namespace mw { namespace exceptions {

class pipe_error : public std::exception {
public:
    pipe_error(const char* func, const std::size_t line, const std::string& msg) {
        std::ostringstream oss;
        oss << func << ":" << line << ": " << msg;
        message = oss.str();
    }

    ~pipe_error() = default;

    const char* what() const noexcept override {
        return message.c_str();
    }

private:
    std::string message;
};

} } // mw::exceptions