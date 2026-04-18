#include <gtest/gtest.h>

#include <mocks/StdLibStaticMock.h>

#include <ipc/SharedSegmentMemoryIpc.h>
#include <exceptions/ftok_error.h>
#include <exceptions/shm_error.h>

#include <filesystem>
#include <fstream>

#include <sys/ipc.h>
#include <sys/sem.h>

#define SHMAT_FAILURE (void *)-1

namespace {

using namespace testing;
using namespace mw::mocks;
using namespace mw::ipc;
using namespace mw::exceptions;

namespace fs = std::filesystem;

constexpr const char* MEMORY_FILE = "memory.file";
constexpr int FAILURE = -1;
constexpr int SUCCESS = 0;
constexpr int PERMISSIONS = 0666;
constexpr int SHM_FLAGS = PERMISSIONS | IPC_CREAT | IPC_EXCL;
constexpr size_t MEMORY_SIZE = 1024;

class SharedSegmentMemoryIpc_tests : public Test {
public:
    ~SharedSegmentMemoryIpc_tests() = default;

protected:
    void SetUp() {
        auto& stdLib = StdLibStaticMock::get();
        const pid_t pid = 1234;
        EXPECT_CALL(stdLib, getpid()).WillRepeatedly(Return(pid));
    }

    void TearDown() {
        if (fs::exists(MEMORY_FILE)) {
            fs::remove(MEMORY_FILE);
        }
    }
};

TEST_F(SharedSegmentMemoryIpc_tests, creator_open_success) {
    auto& stdLib = StdLibStaticMock::get();
    const key_t key = 4321;
    const int shmId = 888;

    EXPECT_CALL(stdLib, ftok(_, _)).WillOnce(Return(key));
    EXPECT_CALL(stdLib, shmget(Eq(key), Eq(MEMORY_SIZE), Eq(SHM_FLAGS))).WillOnce(Return(shmId));
    EXPECT_CALL(stdLib, shmctl(Eq(shmId), Eq(IPC_RMID), Eq(nullptr))).WillOnce(Return(SUCCESS));

    try {
        auto instance = SharedSegmentMemoryIpc{MEMORY_FILE, MEMORY_SIZE, EUsageShmSegment::CREATOR};
        instance.open();
        EXPECT_TRUE(fs::exists(MEMORY_FILE));
    } catch (...) {
        EXPECT_FALSE(true);
    }
    EXPECT_FALSE(fs::exists(MEMORY_FILE));
}

TEST_F(SharedSegmentMemoryIpc_tests, creator_open_ftok_failed) {
    auto& stdLib = StdLibStaticMock::get();

    EXPECT_CALL(stdLib, ftok(_, _)).WillOnce(Return(FAILURE));

    try {
        auto instance = SharedSegmentMemoryIpc{MEMORY_FILE, MEMORY_SIZE, EUsageShmSegment::CREATOR};
        instance.open();
        EXPECT_FALSE(true);
    } catch (const ftok_error& e) {
        std::cout << __PRETTY_FUNCTION__ << ":" << __LINE__ << ": " << e.what() << std::endl;
        EXPECT_TRUE(true);
    } catch (...) {
        EXPECT_FALSE(true);
    }
}

TEST_F(SharedSegmentMemoryIpc_tests, creator_open_semget_failed) {
    auto& stdLib = StdLibStaticMock::get();
    const key_t key = 1234;

    EXPECT_CALL(stdLib, ftok(_, _)).WillOnce(Return(key));
    EXPECT_CALL(stdLib, shmget(Eq(key), Eq(MEMORY_SIZE), Eq(SHM_FLAGS))).WillOnce(Return(FAILURE));

    try {
        auto instance = SharedSegmentMemoryIpc{MEMORY_FILE, MEMORY_SIZE, EUsageShmSegment::CREATOR};
        instance.open();
        EXPECT_FALSE(true);
    } catch (const shm_error& e) {
        std::cout << __PRETTY_FUNCTION__ << ":" << __LINE__ << ": " << e.what() << std::endl;
        EXPECT_TRUE(true);
    } catch (...) {
        EXPECT_FALSE(true);
    }
}

TEST_F(SharedSegmentMemoryIpc_tests, client_open_success) {
    auto& stdLib = StdLibStaticMock::get();
    const key_t key = 1234;
    const int shmId = 888;
    std::array<char, MEMORY_SIZE> shmMem{};

    EXPECT_CALL(stdLib, ftok(_, _)).WillOnce(Return(key));
    EXPECT_CALL(stdLib, shmget(Eq(key), Eq(MEMORY_SIZE), Eq(PERMISSIONS))).WillOnce(Return(shmId));
    EXPECT_CALL(stdLib, shmat(Eq(shmId), Eq(nullptr), Eq(0))).WillOnce(Return(shmMem.data()));
    EXPECT_CALL(stdLib, shmdt(Eq(shmMem.data()))).WillOnce(Return(SUCCESS));

    try {
        auto instance = SharedSegmentMemoryIpc{MEMORY_FILE, MEMORY_SIZE, EUsageShmSegment::CLIENT};
        instance.open();
        EXPECT_TRUE(true);
    } catch (...) {
        EXPECT_FALSE(true);
    }
}

TEST_F(SharedSegmentMemoryIpc_tests, client_open_ftok_failure) {
    auto& stdLib = StdLibStaticMock::get();

    EXPECT_CALL(stdLib, ftok(_, _)).WillOnce(Return(FAILURE));

    try {
        auto instance = SharedSegmentMemoryIpc{MEMORY_FILE, MEMORY_SIZE, EUsageShmSegment::CLIENT};
        instance.open();
        EXPECT_FALSE(true);
    } catch (const ftok_error& e) {
        std::cout << __PRETTY_FUNCTION__ << ":" << __LINE__ << ": " << e.what() << std::endl;
        EXPECT_TRUE(true);
    } catch (...) {
        EXPECT_FALSE(true);
    }
}

TEST_F(SharedSegmentMemoryIpc_tests, client_open_shmget_failed) {
    auto& stdLib = StdLibStaticMock::get();
    const key_t key = 1234;

    EXPECT_CALL(stdLib, ftok(_, _)).WillOnce(Return(key));
    EXPECT_CALL(stdLib, shmget(Eq(key), Eq(MEMORY_SIZE), Eq(PERMISSIONS))).WillOnce(Return(FAILURE));

    try {
        auto instance = SharedSegmentMemoryIpc{MEMORY_FILE, MEMORY_SIZE, EUsageShmSegment::CLIENT};
        instance.open();
        EXPECT_FALSE(true);
    } catch (const shm_error& e) {
        std::cout << __PRETTY_FUNCTION__ << ":" << __LINE__ << ": " << e.what() << std::endl;
        EXPECT_TRUE(true);
    } catch (...) {
        EXPECT_FALSE(true);
    }
}

TEST_F(SharedSegmentMemoryIpc_tests, client_open_shmat_failure) {
    auto& stdLib = StdLibStaticMock::get();
    const key_t key = 1234;
    const int shmId = 888;

    EXPECT_CALL(stdLib, ftok(_, _)).WillOnce(Return(key));
    EXPECT_CALL(stdLib, shmget(Eq(key), Eq(MEMORY_SIZE), Eq(PERMISSIONS))).WillOnce(Return(shmId));
    EXPECT_CALL(stdLib, shmat(Eq(shmId), Eq(nullptr), Eq(0))).WillOnce(Return(SHMAT_FAILURE));
    EXPECT_CALL(stdLib, shmdt(Eq(SHMAT_FAILURE))).WillOnce(Return(FAILURE));

    try {
        auto instance = SharedSegmentMemoryIpc{MEMORY_FILE, MEMORY_SIZE, EUsageShmSegment::CLIENT};
        instance.open();
        EXPECT_FALSE(true);
    } catch (const shm_error& e) {
        std::cout << __PRETTY_FUNCTION__ << ":" << __LINE__ << ": " << e.what() << std::endl;
        EXPECT_TRUE(true);
    } catch (...) {
        EXPECT_FALSE(true);
    }
}

TEST_F(SharedSegmentMemoryIpc_tests, unknown_open) {
    auto instance = SharedSegmentMemoryIpc{MEMORY_FILE, MEMORY_SIZE, EUsageShmSegment::UNKNOWN};

    try {
        instance.open();
        EXPECT_FALSE(true);
    } catch (const shm_error& e) {
        std::cout << __PRETTY_FUNCTION__ << ":" << __LINE__ << ": " << e.what() << std::endl;
        EXPECT_TRUE(true);
    } catch (...) {
        EXPECT_FALSE(true);
    }
}

TEST_F(SharedSegmentMemoryIpc_tests, size_0_open) {
    auto instance = SharedSegmentMemoryIpc{MEMORY_FILE, 0, EUsageShmSegment::CREATOR};

    try {
        instance.open();
        EXPECT_FALSE(true);
    } catch (const shm_error& e) {
        std::cout << __PRETTY_FUNCTION__ << ":" << __LINE__ << ": " << e.what() << std::endl;
        EXPECT_TRUE(true);
    } catch (...) {
        EXPECT_FALSE(true);
    }
}

TEST_F(SharedSegmentMemoryIpc_tests, open_empty_file) {
    auto instance = SharedSegmentMemoryIpc{std::string{""}, MEMORY_SIZE, EUsageShmSegment::CREATOR};

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

TEST_F(SharedSegmentMemoryIpc_tests, creator_close_success) {
    auto& stdLib = StdLibStaticMock::get();
    const key_t key = 4321;
    const int shmId = 888;

    EXPECT_CALL(stdLib, ftok(_, _)).WillOnce(Return(key));
    EXPECT_CALL(stdLib, shmget(Eq(key), Eq(MEMORY_SIZE), Eq(SHM_FLAGS))).WillOnce(Return(shmId));
    EXPECT_CALL(stdLib, shmctl(Eq(shmId), Eq(IPC_RMID), Eq(nullptr))).WillOnce(Return(SUCCESS));

    auto instance = SharedSegmentMemoryIpc{MEMORY_FILE, MEMORY_SIZE, EUsageShmSegment::CREATOR};
    instance.open();

    try {
        instance.close();
        EXPECT_FALSE(fs::exists(MEMORY_FILE));
    } catch (...) {
        EXPECT_FALSE(true);
    }
}

TEST_F(SharedSegmentMemoryIpc_tests, creator_close_shmctl_failure) {
    auto& stdLib = StdLibStaticMock::get();
    const key_t key = 4321;
    const int shmId = 888;

    EXPECT_CALL(stdLib, ftok(_, _)).WillOnce(Return(key));
    EXPECT_CALL(stdLib, shmget(Eq(key), Eq(MEMORY_SIZE), Eq(SHM_FLAGS))).WillOnce(Return(shmId));
    EXPECT_CALL(stdLib, shmctl(Eq(shmId), Eq(IPC_RMID), Eq(nullptr))).WillOnce(Return(FAILURE));

    auto instance = SharedSegmentMemoryIpc{MEMORY_FILE, MEMORY_SIZE, EUsageShmSegment::CREATOR};
    instance.open();

    try {
        instance.close();
        EXPECT_FALSE(true);
    } catch (const shm_error& e) {
        std::cout << __PRETTY_FUNCTION__ << ":" << __LINE__ << ": " << e.what() << std::endl;
        EXPECT_TRUE(true);
    } catch (...) {
        EXPECT_FALSE(true);
    }
}

TEST_F(SharedSegmentMemoryIpc_tests, client_close_success) {
    auto& stdLib = StdLibStaticMock::get();
    const key_t key = 4321;
    const int shmId = 888;
    std::array<char, MEMORY_SIZE> shmMem{};

    EXPECT_CALL(stdLib, ftok(_, _)).WillOnce(Return(key));
    EXPECT_CALL(stdLib, shmget(Eq(key), Eq(MEMORY_SIZE), Eq(PERMISSIONS))).WillOnce(Return(shmId));
    EXPECT_CALL(stdLib, shmat(Eq(shmId), Eq(nullptr), Eq(0))).WillOnce(Return(shmMem.data()));
    EXPECT_CALL(stdLib, shmdt(Eq(shmMem.data()))).WillOnce(Return(SUCCESS));

    auto instance = SharedSegmentMemoryIpc{MEMORY_FILE, MEMORY_SIZE, EUsageShmSegment::CLIENT};
    instance.open();

    try {
        instance.close();
        EXPECT_TRUE(true);
    } catch (...) {
        EXPECT_FALSE(true);
    }
}

TEST_F(SharedSegmentMemoryIpc_tests, client_close_shmdt_failure) {
    auto& stdLib = StdLibStaticMock::get();
    const key_t key = 4321;
    const int shmId = 888;
    std::array<char, MEMORY_SIZE> shmMem{};

    EXPECT_CALL(stdLib, ftok(_, _)).WillOnce(Return(key));
    EXPECT_CALL(stdLib, shmget(Eq(key), Eq(MEMORY_SIZE), Eq(PERMISSIONS))).WillOnce(Return(shmId));
    EXPECT_CALL(stdLib, shmat(Eq(shmId), Eq(nullptr), Eq(0))).WillOnce(Return(shmMem.data()));
    EXPECT_CALL(stdLib, shmdt(Eq(shmMem.data()))).WillOnce(Return(FAILURE));

    auto instance = SharedSegmentMemoryIpc{MEMORY_FILE, MEMORY_SIZE, EUsageShmSegment::CLIENT};
    instance.open();

    try {
        instance.close();
        EXPECT_FALSE(true);
    } catch (const shm_error& e) {
        std::cout << __PRETTY_FUNCTION__ << ":" << __LINE__ << ": " << e.what() << std::endl;
        EXPECT_TRUE(true);
    } catch (...) {
        EXPECT_FALSE(true);
    }
}

TEST_F(SharedSegmentMemoryIpc_tests, creator_read) {
    auto instance = SharedSegmentMemoryIpc{MEMORY_FILE, MEMORY_SIZE, EUsageShmSegment::CREATOR};
    try {
        instance.read();
        EXPECT_FALSE(true);
    } catch (const shm_error& e) {
        std::cout << __PRETTY_FUNCTION__ << ":" << __LINE__ << ": " << e.what() << std::endl;
        EXPECT_TRUE(true);
    }
}

TEST_F(SharedSegmentMemoryIpc_tests, creator_write) {
    auto instance = SharedSegmentMemoryIpc{MEMORY_FILE, MEMORY_SIZE, EUsageShmSegment::CREATOR};
    try {
        instance.write(std::string{"abc"});
        EXPECT_FALSE(true);
    } catch (const shm_error& e) {
        std::cout << __PRETTY_FUNCTION__ << ":" << __LINE__ << ": " << e.what() << std::endl;
        EXPECT_TRUE(true);
    } catch (...) {
        EXPECT_FALSE(true);
    }
}

TEST_F(SharedSegmentMemoryIpc_tests, client_write_success) {
    auto& stdLib = StdLibStaticMock::get();
    const key_t key = 4321;
    const int shmId = 888;
    const std::string msg = "Hello world";
    std::array<char, MEMORY_SIZE> shmMem{};

    EXPECT_CALL(stdLib, ftok(_, _)).WillOnce(Return(key));
    EXPECT_CALL(stdLib, shmget(Eq(key), Eq(MEMORY_SIZE), Eq(PERMISSIONS))).WillOnce(Return(shmId));
    EXPECT_CALL(stdLib, shmat(Eq(shmId), Eq(nullptr), Eq(0))).WillOnce(Return(shmMem.data()));
    EXPECT_CALL(stdLib, shmdt(Eq(shmMem.data()))).WillOnce(Return(SUCCESS));

    auto instance = SharedSegmentMemoryIpc{MEMORY_FILE, MEMORY_SIZE, EUsageShmSegment::CLIENT};
    instance.open();

    try {
        EXPECT_TRUE(instance.write(msg));
        EXPECT_STREQ(msg.data(), shmMem.data());
    } catch (...) {
        EXPECT_FALSE(true);
    }
}

TEST_F(SharedSegmentMemoryIpc_tests, client_write_shmMem_nullptr) {
    auto& stdLib = StdLibStaticMock::get();
    const key_t key = 4321;
    const int shmId = 888;
    const std::string msg = "Hello world";

    EXPECT_CALL(stdLib, ftok(_, _)).WillOnce(Return(key));
    EXPECT_CALL(stdLib, shmget(Eq(key), Eq(MEMORY_SIZE), Eq(PERMISSIONS))).WillOnce(Return(shmId));
    EXPECT_CALL(stdLib, shmat(Eq(shmId), Eq(nullptr), Eq(0))).WillOnce(Return(nullptr));
    EXPECT_CALL(stdLib, shmdt(Eq(nullptr))).WillOnce(Return(SUCCESS));

    auto instance = SharedSegmentMemoryIpc{MEMORY_FILE, MEMORY_SIZE, EUsageShmSegment::CLIENT};
    instance.open();

    try {
        EXPECT_TRUE(instance.write(msg));
    } catch (const shm_error& e) {
        std::cout << __PRETTY_FUNCTION__ << ":" << __LINE__ << ": " << e.what() << std::endl;
        EXPECT_TRUE(true);
    } catch (...) {
        EXPECT_FALSE(true);
    }
}

TEST_F(SharedSegmentMemoryIpc_tests, client_write_message_too_big) {
    auto& stdLib = StdLibStaticMock::get();
    const key_t key = 4321;
    const int shmId = 888;
    const size_t size = 2;
    const std::string msg = "Hello world";
    std::array<char, size> shmMem{};

    EXPECT_CALL(stdLib, ftok(_, _)).WillOnce(Return(key));
    EXPECT_CALL(stdLib, shmget(Eq(key), Eq(size), Eq(PERMISSIONS))).WillOnce(Return(shmId));
    EXPECT_CALL(stdLib, shmat(Eq(shmId), Eq(nullptr), Eq(0))).WillOnce(Return(shmMem.data()));
    EXPECT_CALL(stdLib, shmdt(Eq(shmMem.data()))).WillOnce(Return(SUCCESS));

    auto instance = SharedSegmentMemoryIpc{MEMORY_FILE, size, EUsageShmSegment::CLIENT};
    instance.open();

    try {
        EXPECT_TRUE(instance.write(msg));
    } catch (const shm_error& e) {
        std::cout << __PRETTY_FUNCTION__ << ":" << __LINE__ << ": " << e.what() << std::endl;
        EXPECT_TRUE(true);
    } catch (...) {
        EXPECT_FALSE(true);
    }
}

TEST_F(SharedSegmentMemoryIpc_tests, client_write_message_equal_zero) {
    auto& stdLib = StdLibStaticMock::get();
    const key_t key = 4321;
    const int shmId = 888;
    const std::string msg{""};
    std::array<char, MEMORY_SIZE> shmMem{};

    EXPECT_CALL(stdLib, ftok(_, _)).WillOnce(Return(key));
    EXPECT_CALL(stdLib, shmget(Eq(key), Eq(MEMORY_SIZE), Eq(PERMISSIONS))).WillOnce(Return(shmId));
    EXPECT_CALL(stdLib, shmat(Eq(shmId), Eq(nullptr), Eq(0))).WillOnce(Return(shmMem.data()));
    EXPECT_CALL(stdLib, shmdt(Eq(shmMem.data()))).WillOnce(Return(SUCCESS));

    auto instance = SharedSegmentMemoryIpc{MEMORY_FILE, MEMORY_SIZE, EUsageShmSegment::CLIENT};
    instance.open();

    try {
        EXPECT_TRUE(instance.write(msg));
    } catch (const shm_error& e) {
        std::cout << __PRETTY_FUNCTION__ << ":" << __LINE__ << ": " << e.what() << std::endl;
        EXPECT_TRUE(true);
    } catch (...) {
        EXPECT_FALSE(true);
    }
}

TEST_F(SharedSegmentMemoryIpc_tests, client_read_success) {
    auto& stdLib = StdLibStaticMock::get();
    const key_t key = 4321;
    const int shmId = 888;
    std::string msg = "Hello world";

    EXPECT_CALL(stdLib, ftok(_, _)).WillOnce(Return(key));
    EXPECT_CALL(stdLib, shmget(Eq(key), Eq(msg.size()), Eq(PERMISSIONS))).WillOnce(Return(shmId));
    EXPECT_CALL(stdLib, shmat(Eq(shmId), Eq(nullptr), Eq(0))).WillOnce(Return(msg.data()));
    EXPECT_CALL(stdLib, shmdt(Eq(msg.data()))).WillOnce(Return(SUCCESS));

    auto instance = SharedSegmentMemoryIpc{MEMORY_FILE, msg.size(), EUsageShmSegment::CLIENT};
    instance.open();

    try {
        EXPECT_STREQ(instance.read().c_str(), msg.c_str());
    } catch (...) {
        EXPECT_FALSE(true);
    }
}

TEST_F(SharedSegmentMemoryIpc_tests, client_read_shmMem_nullptr) {
    auto& stdLib = StdLibStaticMock::get();
    const key_t key = 4321;
    const int shmId = 888;

    EXPECT_CALL(stdLib, ftok(_, _)).WillOnce(Return(key));
    EXPECT_CALL(stdLib, shmget(Eq(key), Eq(MEMORY_SIZE),Eq(PERMISSIONS))).WillOnce(Return(shmId));
    EXPECT_CALL(stdLib, shmat(Eq(shmId), Eq(nullptr), Eq(0))).WillOnce(Return(nullptr));
    EXPECT_CALL(stdLib, shmdt(Eq(nullptr))).WillOnce(Return(SUCCESS));

    auto instance = SharedSegmentMemoryIpc{MEMORY_FILE, MEMORY_SIZE, EUsageShmSegment::CLIENT};
    instance.open();

    try {
        instance.read();
        EXPECT_FALSE(true);
    } catch (const shm_error& e) {
        std::cout << __PRETTY_FUNCTION__ << ":" << __LINE__ << ": " << e.what() << std::endl;
        EXPECT_TRUE(true);
    } catch (...) {
        EXPECT_FALSE(true);
    }
}

} // anonymous