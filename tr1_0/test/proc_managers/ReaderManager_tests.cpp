#include <gtest/gtest.h>

#include <proc_managers/ReaderManager.h>

#include <mocks/ipc/SemaphoreIpcMock.h>
#include <mocks/proc_managers/workers/WorkerMock.h>

namespace {

using namespace testing;
using namespace mw::mocks;
using namespace mw::proc_managers;

class ReaderManager_tests : public Test {
public:
    ~ReaderManager_tests() = default;

protected:
    SemaphoreIpcMock dataLocker;
    SemaphoreIpcMock readerLocker;
    WorkerMock worker;
};

TEST_F(ReaderManager_tests, worker_not_started) {
    ReaderManager manager{dataLocker, readerLocker, worker};

    EXPECT_CALL(worker, startWorking()).Times(1);
    EXPECT_CALL(worker, isWorking()).WillOnce(Return(false));

    manager.loop();
}

TEST_F(ReaderManager_tests, worker_started_no_exceptions) {
    ReaderManager manager{dataLocker, readerLocker, worker};

    EXPECT_CALL(worker, startWorking()).Times(1);
    EXPECT_CALL(worker, isWorking())
        .WillOnce(Return(true))
        .WillOnce(Return(false));
    EXPECT_CALL(readerLocker, wait()).Times(1);
    EXPECT_CALL(dataLocker, wait()).Times(1);
    EXPECT_CALL(worker, processData()).Times(1);
    EXPECT_CALL(dataLocker, post()).Times(1);

    manager.loop();
}

TEST_F(ReaderManager_tests, worker_throws_exception_during_starting) {
    ReaderManager manager{dataLocker, readerLocker, worker};

    EXPECT_CALL(worker, startWorking()).WillOnce(Throw(std::runtime_error("test_exception")));
    EXPECT_CALL(worker, stopWorking()).Times(1);

    manager.loop();
}

TEST_F(ReaderManager_tests, worker_throws_exception_inside_while_loop) {
    ReaderManager manager{dataLocker, readerLocker, worker};

    EXPECT_CALL(worker, startWorking()).Times(1);
    EXPECT_CALL(worker, isWorking()).WillOnce(Return(true));
    EXPECT_CALL(readerLocker, wait()).Times(1);
    EXPECT_CALL(dataLocker, wait()).Times(1);
    EXPECT_CALL(worker, processData()).WillOnce(Throw(std::runtime_error("test exception")));
    EXPECT_CALL(worker, stopWorking()).Times(1);

    manager.loop();
}

} // anonymous