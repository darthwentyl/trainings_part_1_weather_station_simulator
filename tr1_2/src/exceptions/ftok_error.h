#pragma once

#include <exception>
#include <string>
#include <sstream>
#include <cstring>

namespace mw { namespace exceptions {

class ftok_error : public std::exception {
public:
    ftok_error(const char* func, const std::size_t line, const std::string& name, const int err) {
        std::ostringstream oss;
        oss << func << ":" << line << ": ftok(" << name << ", " << name.length() << ") failed. err: " << strerror(err);
        message = oss.str();
    }
    ~ftok_error() = default;

    const char* what() const noexcept override {
        return message.c_str();
    }

private:
    std::string message;
};

} } // mw::exceptions