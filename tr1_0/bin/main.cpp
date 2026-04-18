#include <iostream>
#include <memory>

#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <signal.h>

#include <logger/Log.h>
#include <ipc/SharedSegmentMemoryIpc.h>
#include <ipc/SharedSegmentSemaphoreIpc.h>

using namespace mw::ipc;

namespace {
    constexpr const char* mutex_sem_name = "mutex.sem";
    constexpr const char* items_sem_name = "items.sem";
    constexpr const char* mem_name = "memory";
    constexpr size_t mem_size = 128;
} // anonymous


int main() {
    SharedSegmentSemaphoreIpc mutex_main_sem{std::string{mutex_sem_name}, EUsageShmSegment::CREATOR};
    SharedSegmentSemaphoreIpc items_main_sem{std::string{items_sem_name}, EUsageShmSegment::CREATOR};
    SharedSegmentMemoryIpc main_mem{std::string{mem_name}, mem_size, EUsageShmSegment::CREATOR};

    try {
        mutex_main_sem.open(1);
        items_main_sem.open(0);
        main_mem.open();
    } catch(const std::exception& e) {
        ERROR(e.what());
        exit(-1);
    }

    pid_t pid0, pid1, pid2;
    pid0 = fork();
    if (pid0 == 0) {
        try {
            // INFO("writer pid " << getpid());
            SharedSegmentSemaphoreIpc mutex_sem{std::string{mutex_sem_name}, EUsageShmSegment::CLIENT};
            SharedSegmentSemaphoreIpc items_sem{std::string{items_sem_name}, EUsageShmSegment::CLIENT};
            SharedSegmentMemoryIpc mem{std::string{mem_name}, mem_size, EUsageShmSegment::CLIENT};
            mutex_sem.open();
            items_sem.open();
            mem.open();
            for (size_t i = 0; i < 3; ++i) {
                mutex_sem.wait();
                std::stringstream ss;
                ss << "Message " << i;
                INFO("WRITER  : " << ss.str());
                mem.write(ss.str());
                mutex_sem.post();
                items_sem.post();
                items_sem.post();
                sleep(1);
            }

        } catch(const std::exception& e) {
            ERROR(e.what());
        }
        exit(0);
    }

    pid1 = fork();
    if (pid1 == 0) {
        try {
            SharedSegmentSemaphoreIpc mutex_sem{std::string{mutex_sem_name}, EUsageShmSegment::CLIENT};
            SharedSegmentSemaphoreIpc items_sem{std::string{items_sem_name}, EUsageShmSegment::CLIENT};
            SharedSegmentMemoryIpc mem{std::string{mem_name}, mem_size, EUsageShmSegment::CLIENT};
            mutex_sem.open();
            items_sem.open();
            mem.open();
            for (size_t i = 0; i < 3; ++i) {
                items_sem.wait();
                mutex_sem.wait();
                INFO("\t\tREADER 1: " << mem.read());
                mutex_sem.post();
            }

        } catch(const std::exception& e) {
            ERROR(e.what());
        }
        exit(0);
    }
    pid2 = fork();
    if (pid2 == 0) {
        // INFO("reader 2 pid " << getpid());
        try {
            SharedSegmentSemaphoreIpc mutex_sem{std::string{mutex_sem_name}, EUsageShmSegment::CLIENT};
            SharedSegmentSemaphoreIpc items_sem{std::string{items_sem_name}, EUsageShmSegment::CLIENT};
            SharedSegmentMemoryIpc mem{std::string{mem_name}, mem_size, EUsageShmSegment::CLIENT};
            mutex_sem.open();
            items_sem.open();
            mem.open();
            for (size_t i = 0; i < 3; ++i) {
                items_sem.wait();
                mutex_sem.wait();
                // sleep(3);
                INFO("\tREADER 2: " << mem.read());
                mutex_sem.post();
            }
        } catch(const std::exception& e) {
            ERROR(e.what());
        }
        exit(0);
    }

    INFO("I am waiting for finish all child processes");
    for (size_t n = 0; n < 3; ++n) {
        wait(NULL);
    }
    INFO("I am finished");

    return 0;
}