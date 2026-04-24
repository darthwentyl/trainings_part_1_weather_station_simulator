#pragma once

#if defined(UT)

#include <iostream>

#define DEBUG(msg) \
    do { \
        std::cout << __PRETTY_FUNCTION__ << ":" << __LINE__ << ": " << msg << std::endl; \
    } while(false)

#define INFO(msg) \
    do { \
        std::cout << __PRETTY_FUNCTION__ << ":" << __LINE__ << ": " << msg << std::endl; \
    } while(false)

#define ERROR(msg) \
    do { \
        std::cout << __PRETTY_FUNCTION__ << ":" << __LINE__ << ": " << msg << std::endl; \
    } while(false)

#else

#include <sstream>
#include <semaphore.h>
#include <unistd.h>

#if defined(RELEASE)

#define DEBUG(msg)

#else

#define DEBUG(msg) \
    do { \
        std::ostringstream oss; \
        oss << getpid() << ":" << __PRETTY_FUNCTION__ << ":" << __LINE__ << ": " << msg; \
        mw::Log::log().debug(oss); \
    } while(false)

#endif

#define INFO(msg) \
    do { \
        std::ostringstream oss; \
        oss << getpid() << ":" << __PRETTY_FUNCTION__ << ":" << __LINE__ << ": " << msg; \
        mw::Log::log().info(oss); \
    } while(false)

#define ERROR(msg) \
    do { \
        std::ostringstream oss; \
        oss << getpid() << ":" << __PRETTY_FUNCTION__ << ":" << __LINE__ << ": " << msg; \
        mw::Log::log().error(oss); \
    } while(false)

namespace mw {

class Log {
public:
    Log(const Log&) = delete;
    Log& operator = (const Log&) = delete;
    Log(Log&&) = delete;
    Log& operator = (Log&&) = delete;

    static Log& log();

    void debug(const std::ostringstream& msg);
    void info(const std::ostringstream& msg);
    void error(const std::ostringstream& msg);

private:
    Log();
    ~Log();

    sem_t* logSemaphore;
};

} // mw

#endif
