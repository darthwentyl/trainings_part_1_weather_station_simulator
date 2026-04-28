#pragma once

#include <exception>
#include <string>
#include <sstream>
#include <cstring>

namespace mw { namespace exceptions {

class shm_error : public std::exception {
public:
    shm_error(const char* func, const std::size_t line, const std::string& name, const std::size_t size, const int err) {
        std::ostringstream oss;
        oss << func << ":" << line << ": shmget() for name: " << name << "; size: " << size << ") failed. err: " << strerror(err);
        message = oss.str();
    }

    shm_error(const char* func, const std::size_t line, const char* op, const int err) {
        std::ostringstream oss;
        oss << func << ":" << line << ": " << op << "() failed. err: " << strerror(err);
        message = oss.str();
    }

    shm_error(const char* func, const std::size_t line, const char* msg) {
        std::ostringstream oss;
        oss << func << ":" << line << ": " << msg;
        message = oss.str();
    }

    ~shm_error() = default;

    const char* what() const noexcept override {
        return message.c_str();
    }

private:
    std::string message;
};

} } // mw::exceptions