#include <gtest/gtest.h>

#include <mocks/StdLibStaticMock.h>

#include <ipc/SharedSegmentSemaphoreIpc.h>
#include <exceptions/ftok_error.h>
#include <exceptions/sem_error.h>

#include <filesystem>
#include <fstream>

#include <sys/ipc.h>
#include <sys/sem.h>

namespace {

using namespace testing;
using namespace mw::mocks;
using namespace mw::ipc;
using namespace mw::exceptions;

namespace fs = std::filesystem;

constexpr const char* SEMAPHORE_FILE = "sem.file";
constexpr int FAILURE = -1;
constexpr int SUCCESS = 0;
constexpr int PERMISSIONS = 0666;
constexpr int SEM_FLAGS = IPC_CREAT | IPC_EXCL | PERMISSIONS;

class SharedSegmentSemaphoreIpc_tests : public Test {
public:
    ~SharedSegmentSemaphoreIpc_tests() = default;

protected:
    void SetUp() {
        auto& stdLib = StdLibStaticMock::get();
        const pid_t pid = 1234;
        EXPECT_CALL(stdLib, getpid()).WillRepeatedly(Return(pid));
    }

    void TearDown() {
        const std::string semName = std::string{SEMAPHORE_FILE};
        if (fs::exists(semName)) {
            fs::remove(semName);
        }
    }
};

TEST_F(SharedSegmentSemaphoreIpc_tests, creator_open_success) {
    auto& stdLib = StdLibStaticMock::get();
    const key_t key = 4321;
    const int semId = 888;
    const std::string semName = std::string{SEMAPHORE_FILE};

    EXPECT_CALL(stdLib, ftok(_, _)).WillOnce(Return(key));
    EXPECT_CALL(stdLib, semget(Eq(key), _, Eq(SEM_FLAGS))).WillOnce(Return(semId));
    EXPECT_CALL(stdLib, semctl(Eq(semId), _, Eq(SETVAL))).WillOnce(Return(SUCCESS));
    EXPECT_CALL(stdLib, semctl(Eq(semId), _, Eq(IPC_RMID))).WillOnce(Return(SUCCESS));

    try {
        auto instance = SharedSegmentSemaphoreIpc{std::string{SEMAPHORE_FILE}, EUsageShmSegment::CREATOR};
        instance.open(0);
        EXPECT_TRUE(fs::exists(semName));
    } catch (...) {
        EXPECT_TRUE(false);
    }
    EXPECT_FALSE(fs::exists(semName));
}

TEST_F(SharedSegmentSemaphoreIpc_tests, creator_open_file_exists_before_create) {
    auto& stdLib = StdLibStaticMock::get();
    const key_t key = 4321;
    const int semId = 888;
    const std::string semName = std::string{SEMAPHORE_FILE};

    std::ofstream ofs(semName);
    EXPECT_CALL(stdLib, ftok(_, _)).WillOnce(Return(key));
    EXPECT_CALL(stdLib, semget(Eq(key), _, Eq(SEM_FLAGS))).WillOnce(Return(semId));
    EXPECT_CALL(stdLib, semctl(Eq(semId), _, Eq(SETVAL))).WillOnce(Return(SUCCESS));
    EXPECT_CALL(stdLib, semctl(Eq(semId), _, Eq(IPC_RMID))).WillOnce(Return(SUCCESS));

    try {
        EXPECT_TRUE(fs::exists(semName));
        auto instance = SharedSegmentSemaphoreIpc{std::string{SEMAPHORE_FILE}, EUsageShmSegment::CREATOR};
        instance.open(0);
        EXPECT_TRUE(fs::exists(semName));
    } catch (...) {
        EXPECT_TRUE(false);
    }
    EXPECT_FALSE(fs::exists(semName));
}

TEST_F(SharedSegmentSemaphoreIpc_tests, creator_open_ftok_failure) {
    auto& stdLib = StdLibStaticMock::get();

    EXPECT_CALL(stdLib, ftok(_, _)).WillOnce(Return(FAILURE));

    try {
        auto instance = SharedSegmentSemaphoreIpc{std::string{SEMAPHORE_FILE}, EUsageShmSegment::CREATOR};
        instance.open();
        EXPECT_FALSE(true);
    } catch (const ftok_error& e) {
        std::cout << __PRETTY_FUNCTION__<< ":" << __LINE__ << ": " << e.what() << std::endl;
        EXPECT_TRUE(true);
    } catch (...) {
        EXPECT_FALSE(true);
    }
}

TEST_F(SharedSegmentSemaphoreIpc_tests, creator_open_semget_ipc_creat_0666_failure) {
    auto& stdLib = StdLibStaticMock::get();
    const key_t key = 4321;

    EXPECT_CALL(stdLib, ftok(_, _)).WillOnce(Return(key));
    EXPECT_CALL(stdLib, semget(Eq(key), _, Eq(SEM_FLAGS))).WillOnce(Return(FAILURE));

    try {
        auto instance = SharedSegmentSemaphoreIpc{std::string{SEMAPHORE_FILE}, EUsageShmSegment::CREATOR};
        instance.open(1);
        EXPECT_FALSE(true);
    } catch (const sem_error& e) {
        std::cout << __PRETTY_FUNCTION__<< ":" << __LINE__ << ": " << e.what() << std::endl;
        EXPECT_TRUE(true);
    } catch (...) {
        EXPECT_FALSE(true);
    }
}

TEST_F(SharedSegmentSemaphoreIpc_tests, creator_open_semctl_setval_failure) {
    auto& stdLib = StdLibStaticMock::get();
    const key_t key = 4321;
    const int semId = 888;

    EXPECT_CALL(stdLib, ftok(_, _)).WillOnce(Return(key));
    EXPECT_CALL(stdLib, semget(Eq(key), _, Eq(SEM_FLAGS))).WillOnce(Return(semId));
    EXPECT_CALL(stdLib, semctl(Eq(semId), _, Eq(SETVAL))).WillOnce(Return(FAILURE));
    EXPECT_CALL(stdLib, semctl(Eq(semId), _, Eq(IPC_RMID))).WillOnce(Return(SUCCESS));

    try {
        auto instance = SharedSegmentSemaphoreIpc{std::string{SEMAPHORE_FILE}, EUsageShmSegment::CREATOR};
        instance.open(1);
        EXPECT_FALSE(true);
    } catch (const sem_error& e) {
        std::cout << __PRETTY_FUNCTION__<< ":" << __LINE__ << ": " << e.what() << std::endl;
        EXPECT_TRUE(true);
    } catch (...) {
        EXPECT_FALSE(true);
    }
}

TEST_F(SharedSegmentSemaphoreIpc_tests, creator_open_initial_val_not_set) {
    auto& stdLib = StdLibStaticMock::get();
    const key_t key = 4321;

    EXPECT_CALL(stdLib, ftok(_, _)).WillOnce(Return(key));
    try {
        auto instance = SharedSegmentSemaphoreIpc{std::string{SEMAPHORE_FILE}, EUsageShmSegment::CREATOR};
        instance.open();
        EXPECT_FALSE(true);
    } catch (const sem_error& e) {
        std::cout << __PRETTY_FUNCTION__ << ":" << __LINE__ << ": " << e.what() << std::endl;
        EXPECT_TRUE(true);
    } catch (...) {
        EXPECT_FALSE(true);
    }
}

TEST_F(SharedSegmentSemaphoreIpc_tests, creator_wait) {
    try {
        auto instance = SharedSegmentSemaphoreIpc{std::string{SEMAPHORE_FILE}, EUsageShmSegment::CREATOR};
        instance.wait();
        EXPECT_FALSE(true);
    } catch (const sem_error& e) {
        std::cout << __PRETTY_FUNCTION__<< ":" << __LINE__ << ": " << e.what() << std::endl;
        EXPECT_TRUE(true);
    } catch (...) {
        EXPECT_FALSE(true);
    }
}

TEST_F(SharedSegmentSemaphoreIpc_tests, creator_post) {
    try {
        auto instance = SharedSegmentSemaphoreIpc{std::string{SEMAPHORE_FILE}, EUsageShmSegment::CREATOR};
        instance.post();
        EXPECT_FALSE(true);
    } catch (const sem_error& e) {
        std::cout << __PRETTY_FUNCTION__<< ":" << __LINE__ << ": " << e.what() << std::endl;
        EXPECT_TRUE(true);
    } catch (...) {
        EXPECT_FALSE(true);
    }
}

TEST_F(SharedSegmentSemaphoreIpc_tests, creator_close_success) {
    auto& stdLib = StdLibStaticMock::get();
    const key_t key = 4321;
    const int semId = 888;
    const std::string semName = std::string{SEMAPHORE_FILE};

    EXPECT_CALL(stdLib, ftok(_, _)).WillOnce(Return(key));
    EXPECT_CALL(stdLib, semget(Eq(key), _, Eq(SEM_FLAGS))).WillOnce(Return(semId));
    EXPECT_CALL(stdLib, semctl(Eq(semId), _, Eq(SETVAL))).WillOnce(Return(SUCCESS));
    EXPECT_CALL(stdLib, semctl(Eq(semId), _, Eq(IPC_RMID))).WillOnce(Return(SUCCESS));

    auto instance = SharedSegmentSemaphoreIpc{std::string{SEMAPHORE_FILE}, EUsageShmSegment::CREATOR};
    instance.open(1);
    EXPECT_TRUE(fs::exists(semName));
    try {
        instance.close();
        EXPECT_TRUE(true);
    } catch (...) {
        EXPECT_FALSE(true);
    }
    EXPECT_FALSE(fs::exists(semName));
}

TEST_F(SharedSegmentSemaphoreIpc_tests, creator_close_ipc_rmid_failure) {
    auto& stdLib = StdLibStaticMock::get();
    const key_t key = 4321;
    const int semId = 888;
    const std::string semName = std::string{SEMAPHORE_FILE};

    EXPECT_CALL(stdLib, ftok(_, _)).WillOnce(Return(key));
    EXPECT_CALL(stdLib, semget(Eq(key), _, Eq(SEM_FLAGS))).WillOnce(Return(semId));
    EXPECT_CALL(stdLib, semctl(Eq(semId), _, Eq(SETVAL))).WillOnce(Return(SUCCESS));
    EXPECT_CALL(stdLib, semctl(Eq(semId), _, Eq(IPC_RMID))).WillOnce(Return(FAILURE));

    auto instance = SharedSegmentSemaphoreIpc{std::string{SEMAPHORE_FILE}, EUsageShmSegment::CREATOR};
    instance.open(1);
    EXPECT_TRUE(fs::exists(semName));
    try {
        instance.close();
        EXPECT_FALSE(true);
    } catch (const sem_error& e) {
        std::cout << __PRETTY_FUNCTION__<< ":" << __LINE__ << ": " << e.what() << std::endl;
        EXPECT_TRUE(true);
    } catch (...) {
        EXPECT_FALSE(true);
    }
}

TEST_F(SharedSegmentSemaphoreIpc_tests, creator_close_throw_during_destruction) {
    auto& stdLib = StdLibStaticMock::get();
    const key_t key = 4321;
    const int semId = 888;
    const std::string semName = std::string{SEMAPHORE_FILE};

    EXPECT_CALL(stdLib, ftok(_, _)).WillOnce(Return(key));
    EXPECT_CALL(stdLib, semget(Eq(key), _, Eq(SEM_FLAGS))).WillOnce(Return(semId));
    EXPECT_CALL(stdLib, semctl(Eq(semId), _, Eq(SETVAL))).WillOnce(Return(SUCCESS));
    EXPECT_CALL(stdLib, semctl(Eq(semId), _, Eq(IPC_RMID))).WillOnce(Return(FAILURE));

    {
        auto instance = SharedSegmentSemaphoreIpc{std::string{SEMAPHORE_FILE}, EUsageShmSegment::CREATOR};
        instance.open(1);
        EXPECT_TRUE(fs::exists(semName));
    }
    // if close throw exception, the file is not removed
    EXPECT_TRUE(fs::exists(semName));
}

TEST_F(SharedSegmentSemaphoreIpc_tests, client_open_success) {
    auto& stdLib = StdLibStaticMock::get();
    const key_t key = 4321;
    const int semId = 888;

    EXPECT_CALL(stdLib, ftok(_, _)).WillOnce(Return(key));
    EXPECT_CALL(stdLib, semget(Eq(key), _, Eq(PERMISSIONS))).WillOnce(Return(semId));

    try {
        auto instance = SharedSegmentSemaphoreIpc{std::string{SEMAPHORE_FILE}, EUsageShmSegment::CLIENT};
        instance.open();
    } catch (...) {
        EXPECT_TRUE(false);
    }
}

TEST_F(SharedSegmentSemaphoreIpc_tests, client_open_ftok_failure) {
    auto& stdLib = StdLibStaticMock::get();

    EXPECT_CALL(stdLib, ftok(_, _)).WillOnce(Return(FAILURE));

    try {
        auto instance = SharedSegmentSemaphoreIpc{std::string{SEMAPHORE_FILE}, EUsageShmSegment::CLIENT};
        instance.open();
        EXPECT_FALSE(true);
    } catch (const ftok_error& e) {
        std::cout << __PRETTY_FUNCTION__ << ":" << __LINE__ << ": " << e.what() << std::endl;
        EXPECT_TRUE(true);
    } catch (...) {
        EXPECT_FALSE(true);
    }
}

TEST_F(SharedSegmentSemaphoreIpc_tests, client_open_semget_failure) {
    auto& stdLib = StdLibStaticMock::get();
    const key_t key = 4321;

    EXPECT_CALL(stdLib, ftok(_, _)).WillOnce(Return(key));
    EXPECT_CALL(stdLib, semget(Eq(key), _, Eq(PERMISSIONS))).WillOnce(Return(FAILURE));

    try {
        auto instance = SharedSegmentSemaphoreIpc{std::string{SEMAPHORE_FILE}, EUsageShmSegment::CLIENT};
        instance.open();
        EXPECT_FALSE(true);
    } catch (const sem_error& e) {
        std::cout << __PRETTY_FUNCTION__ << ":" << __LINE__ << ": " << e.what() << std::endl;
        EXPECT_TRUE(true);
    } catch (...) {
        EXPECT_FALSE(true);
    }
}

TEST_F(SharedSegmentSemaphoreIpc_tests, client_wait_success) {
    auto& stdLib = StdLibStaticMock::get();
    const key_t key = 4321;
    const int semId = 888;

    EXPECT_CALL(stdLib, ftok(_, _)).WillOnce(Return(key));
    EXPECT_CALL(stdLib, semget(Eq(key), _, Eq(PERMISSIONS))).WillOnce(Return(semId));
    EXPECT_CALL(stdLib, semop(Eq(semId), _, _)).WillOnce(Return(SUCCESS));

    auto instance = SharedSegmentSemaphoreIpc{std::string{SEMAPHORE_FILE}, EUsageShmSegment::CLIENT};
    instance.open();

    try {
        instance.wait();
    } catch (...) {
        EXPECT_TRUE(false);
    }
}

TEST_F(SharedSegmentSemaphoreIpc_tests, client_wait_failure) {
    auto& stdLib = StdLibStaticMock::get();

    EXPECT_CALL(stdLib, semop(Eq(-1), _, _)).WillOnce(Return(FAILURE));

    auto instance = SharedSegmentSemaphoreIpc{std::string{SEMAPHORE_FILE}, EUsageShmSegment::CLIENT};

    try {
        instance.wait();
        EXPECT_FALSE(true);
    } catch (const sem_error& e) {
        std::cout << __PRETTY_FUNCTION__ << ":" << __LINE__ << ": " << e.what() << std::endl;
        EXPECT_TRUE(true);
    } catch (...) {
        EXPECT_FALSE(true);
    }
}

TEST_F(SharedSegmentSemaphoreIpc_tests, client_post_success) {
    auto& stdLib = StdLibStaticMock::get();
    const key_t key = 4321;
    const int semId = 888;

    EXPECT_CALL(stdLib, ftok(_, _)).WillOnce(Return(key));
    EXPECT_CALL(stdLib, semget(Eq(key), _, Eq(PERMISSIONS))).WillOnce(Return(semId));
    EXPECT_CALL(stdLib, semop(Eq(semId), _, _)).WillOnce(Return(SUCCESS));

    auto instance = SharedSegmentSemaphoreIpc{std::string{SEMAPHORE_FILE}, EUsageShmSegment::CLIENT};
    instance.open();

    try {
        instance.post();
    } catch (...) {
        EXPECT_TRUE(false);
    }
}

TEST_F(SharedSegmentSemaphoreIpc_tests, client_post_failure) {
    auto& stdLib = StdLibStaticMock::get();

    EXPECT_CALL(stdLib, semop(Eq(-1), _, _)).WillOnce(Return(FAILURE));

    auto instance = SharedSegmentSemaphoreIpc{std::string{SEMAPHORE_FILE}, EUsageShmSegment::CLIENT};

    try {
        instance.post();
        EXPECT_FALSE(true);
    } catch (const sem_error& e) {
        std::cout << __PRETTY_FUNCTION__ << ":" << __LINE__ << ": " << e.what() << std::endl;
        EXPECT_TRUE(true);
    } catch (...) {
        EXPECT_FALSE(true);
    }
}

TEST_F(SharedSegmentSemaphoreIpc_tests, unknown_open) {
    auto instance = SharedSegmentSemaphoreIpc{std::string{SEMAPHORE_FILE}, EUsageShmSegment::UNKNOWN};

    try {
        instance.open();
        EXPECT_FALSE(true);
    } catch (const sem_error& e) {
        std::cout << __PRETTY_FUNCTION__ << ":" << __LINE__ << ": " << e.what() << std::endl;
        EXPECT_TRUE(true);
    } catch (...) {
        EXPECT_FALSE(true);
    }
}

TEST_F(SharedSegmentSemaphoreIpc_tests, open_empty_file) {
    auto instance = SharedSegmentSemaphoreIpc{std::string{""}, EUsageShmSegment::CREATOR};
    try {
        instance.open();
        EXPECT_FALSE(true);
    } catch (const std::runtime_error& e) {
        std::cout << __PRETTY_FUNCTION__ << ":" << __LINE__ << ": " << e.what() << std::endl;
        EXPECT_TRUE(true);
    } catch (...) {
        EXPECT_FALSE(true);
    }
}

TEST_F(SharedSegmentSemaphoreIpc_tests, creator_already_closed_based_on_usage) {
    auto& stdLib = StdLibStaticMock::get();
    auto instance = SharedSegmentSemaphoreIpc{std::string{""}, EUsageShmSegment::UNKNOWN};
    EXPECT_CALL(stdLib, semctl(_, _, _)).Times(0);
    instance.close();
}

TEST_F(SharedSegmentSemaphoreIpc_tests, creator_already_closed_based_on_semId) {
    auto& stdLib = StdLibStaticMock::get();
    auto instance = SharedSegmentSemaphoreIpc{std::string{""}, EUsageShmSegment::CREATOR};
    EXPECT_CALL(stdLib, semctl(_, _, _)).Times(0);
    instance.close();
}

} // anonymous