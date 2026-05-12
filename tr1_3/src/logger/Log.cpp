#if !defined(UT)

#include <logger/Log.h>

#include <iostream>
#include <sstream>
#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>
#include <cstring>

namespace mw {

static constexpr const char* RESET = "\033[0m";
static constexpr const char* GREEN = "\033[32m";
static constexpr const char* WHITE = "\033[37m";
static constexpr const char* RED   = "\033[31m";
static constexpr const char* LOGGER_SEM_NAME = "/logger_semaphore";

Log& Log::log() {
    static Log instance;
    return instance;
}

void Log::debug(const std::ostringstream& msg) {
    sem_wait(logSemaphore);
    std::cout << GREEN << "[DEBUG] " << RESET << msg.str() << std::endl;
    sem_post(logSemaphore);
}

void Log::info(const std::ostringstream& msg) {
    sem_wait(logSemaphore);
    std::cout << WHITE << "[INFO ] " << RESET << msg.str() << std::endl;
    sem_post(logSemaphore);
}

void Log::error(const std::ostringstream& msg) {
    sem_wait(logSemaphore);
    std::cerr << RED << "[ERROR] " << RESET <<  msg.str() << std::endl;
    sem_post(logSemaphore);
}

Log::Log() {
    logSemaphore = sem_open(LOGGER_SEM_NAME, O_CREAT, 0644, 1);
    if (logSemaphore == SEM_FAILED) {
        std::cerr << "Semaphore creation failed: " << strerror(errno) << std::endl;
        exit(EXIT_FAILURE);
    }
}

Log::~Log() {
    sem_close(logSemaphore);
    sem_unlink(LOGGER_SEM_NAME);
}

} // mw

#endif