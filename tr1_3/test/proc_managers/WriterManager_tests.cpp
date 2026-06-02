#include <gtest/gtest.h>

#include <proc_managers/WriterManager.h>

#include <mocks/ipc/SemaphoreIpcMock.h>
#include <mocks/proc_managers/workers/WorkerMock.h>
#include <mocks/StdLibStaticMock.h>

namespace {

using namespace testing;
using namespace mw::mocks;
using namespace mw::proc_managers;

constexpr const std::size_t readersNum = 2;
constexpr const pid_t testPid = 123;

class WriterManager_tests : public Test {
public:
    ~WriterManager_tests() = default;

protected:
    StrictMock<SemaphoreIpcMock> dataLocker;
    StrictMock<SemaphoreIpcMock> readerLocker;
    StrictMock<WorkerMock> worker;
};

TEST_F(WriterManager_tests, worker_not_started) {
    WriterManager manager{testPid, readersNum, dataLocker, readerLocker, worker};

    EXPECT_CALL(dataLocker, open(_)).Times(1);
    EXPECT_CALL(readerLocker, open(_)).Times(1);
    EXPECT_CALL(worker, startWorking()).Times(1);
    EXPECT_CALL(worker, isWorking()).WillOnce(Return(false));

    manager.loop();
}

TEST_F(WriterManager_tests, worker_started_no_exceptions) {
    WriterManager manager{testPid, readersNum, dataLocker, readerLocker, worker};

    EXPECT_CALL(dataLocker, open(_)).Times(1);
    EXPECT_CALL(readerLocker, open(_)).Times(1);
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

TEST_F(WriterManager_tests, worker_throws_exception_during_starting) {
    auto& stdLib = StdLibStaticMock::get();
    WriterManager manager{testPid, readersNum, dataLocker, readerLocker, worker};

    EXPECT_CALL(dataLocker, open(_)).Times(1);
    EXPECT_CALL(readerLocker, open(_)).Times(1);
    EXPECT_CALL(worker, startWorking()).WillOnce(Throw(std::runtime_error("test exception")));
    EXPECT_CALL(stdLib, kill(Eq(testPid), Eq(SIGINT))).WillOnce(Return(0));

    manager.loop();
}

TEST_F(WriterManager_tests, worker_throws_exception_inside_while_loop) {
    auto& stdLib = StdLibStaticMock::get();
    WriterManager  manager{testPid, readersNum, dataLocker, readerLocker, worker};

    EXPECT_CALL(dataLocker, open(_)).Times(1);
    EXPECT_CALL(readerLocker, open(_)).Times(1);
    EXPECT_CALL(worker, startWorking()).Times(1);
    EXPECT_CALL(worker, isWorking()).WillOnce(Return(true));
    EXPECT_CALL(dataLocker, wait()).Times(1);
    EXPECT_CALL(worker, processData()).WillOnce(Throw(std::runtime_error("test exception")));
    EXPECT_CALL(stdLib, kill(Eq(testPid), Eq(SIGINT))).WillOnce(Return(0));

    manager.loop();
}

} // anonymous