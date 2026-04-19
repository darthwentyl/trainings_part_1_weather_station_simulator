#include <gtest/gtest.h>

#include <proc_managers/WriterManager.h>

#include <mocks/ipc/SemaphoreIpcMock.h>
#include <mocks/proc_managers/workers/WorkerMock.h>

namespace {

constexpr const std::size_t readersNum = 2;

using namespace testing;
using namespace mw::mocks;
using namespace mw::proc_managers;

class WriterManager_tests : public Test {
public:
    ~WriterManager_tests() = default;

protected:
    SemaphoreIpcMock dataLocker;
    SemaphoreIpcMock readerLocker;
    WorkerMock worker;
};

TEST_F(WriterManager_tests, worker_not_started) {
    WriterManager manager{readersNum, dataLocker, readerLocker, worker};

    EXPECT_CALL(worker, startWorking()).Times(1);
    EXPECT_CALL(worker, isWorking()).WillOnce(Return(false));

    manager.loop();
}

TEST_F(WriterManager_tests, worker_started_no_exceptions) {
    WriterManager manager{readersNum, dataLocker, readerLocker, worker};

    EXPECT_CALL(worker, startWorking()).Times(1);
    EXPECT_CALL(worker, isWorking())
        .WillOnce(Return(true))
        .WillOnce(Return(false));
    EXPECT_CALL(dataLocker, wait()).Times(1);
    EXPECT_CALL(worker, processData()).Times(1);
    EXPECT_CALL(dataLocker, post()).Times(1);
    EXPECT_CALL(readerLocker, post()).Times(readersNum);

    manager.loop();
}

TEST_F(WriterManager_tests, worker_started_throw_exception_outside_while_loop) {
    WriterManager manager{readersNum, dataLocker, readerLocker, worker};

    EXPECT_CALL(worker, startWorking()).WillOnce(Throw(std::runtime_error("test exception")));
    EXPECT_CALL(worker, stopWorking()).Times(1);

    manager.loop();
}

TEST_F(WriterManager_tests, worker_started_throw_exception_inside_while_loop)  {
    WriterManager  manager{readersNum, dataLocker, readerLocker, worker};

    EXPECT_CALL(worker, startWorking()).Times(1);
    EXPECT_CALL(worker, isWorking()).WillOnce(Return(true));
    EXPECT_CALL(dataLocker, wait()).Times(1);
    EXPECT_CALL(worker, processData()).WillOnce(Throw(std::runtime_error("test exception")));
    EXPECT_CALL(worker, stopWorking()).Times(1);

    manager.loop();
}

} // anonymous