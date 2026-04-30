#pragma once

#include <gmock/gmock.h>
#include <semaphore.h>

namespace mw { namespace mocks {

class StdLibStaticMock {
public:
    ~StdLibStaticMock() = default;

    StdLibStaticMock(const StdLibStaticMock&) = delete;
    StdLibStaticMock(StdLibStaticMock&&) = delete;
    StdLibStaticMock& operator = (const StdLibStaticMock&) = delete;
    StdLibStaticMock& operator = (StdLibStaticMock&&) = delete;

    static testing::StrictMock<StdLibStaticMock>& get();

    MOCK_METHOD(key_t, ftok, (const char* pathname, int proj_id), ());
    MOCK_METHOD(int, semget, (key_t key, int nsems, int semflg), ());
    MOCK_METHOD(int, semctl, (int semid, int semnum, int op), ());
    MOCK_METHOD(int, semop, (int semid, struct sembuf* sops, size_t nsops), ());
    MOCK_METHOD(pid_t, getpid, (), ());
    MOCK_METHOD(int, shmget, (key_t key, size_t size, int shmflg), ());
    MOCK_METHOD(void*, shmat, (int shmid, const void* shmaddr, int shmflg), ());
    MOCK_METHOD(int, shmctl, (int shmid, int op, struct shmid_ds *buf), ());
    MOCK_METHOD(int, shmdt, (const void* shmaddr), ());
    MOCK_METHOD(FILE*, popen, (const char* command, const char* type), ());
    MOCK_METHOD(int, pclose, (FILE* stream), ());

private:
    friend class testing::StrictMock<StdLibStaticMock>;
    StdLibStaticMock();
};

} } // mw::mocks