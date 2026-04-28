#pragma once

#include <exception>
#include <string>
#include <sstream>
#include <cstring>

namespace mw { namespace exceptions {

class sem_error : public std::exception {
public:
    sem_error(const char* func, const std::size_t line, const char* op, const int err) {
        std::ostringstream oss;
        oss << func << ":" << line << ": " << op << "() failed. err: " << strerror(err);
        message = oss.str();
    }

    sem_error(const char* func, const std::size_t line, const char* msg) {
        std::ostringstream oss;
        oss << func << ":" << line << ": " << msg;
        message = oss.str();
    }

    ~sem_error() = default;

    const char* what() const noexcept override {
        return message.c_str();
    }

private:
    std::string message;
};

} } // mw::exceptions