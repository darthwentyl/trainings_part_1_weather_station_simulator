#pragma once

#include <exception>
#include <string>
#include <sstream>

namespace mw { namespace exceptions {

class fixed_lines_file_writer_error : public std::exception {
public:
    fixed_lines_file_writer_error(const char* func, const std::size_t line, const std::string& message) {
        std::ostringstream oss;
        oss << func << ":" << line << ": " << message;
        this->message = oss.str();
    }

    const char* what() const noexcept override {
        return message.c_str();
    }

private:
    std::string message;
};

} } // mw::exceptions