#include <gtest/gtest.h>

#include <mocks/ipc/IpcMock.h>

#include <proc_managers/workers/ExitWorker.h>

namespace {

using namespace testing;
using namespace mw::mocks;
using namespace mw::proc_managers::workers;

constexpr const char* exit_msg = "exit";

class ExitWorker_tests : public Test {
public:
    ~ExitWorker_tests() = default;

protected:
    void TearDown() {
        worker = nullptr;
    }

    StrictMock<IpcMock> ipcMock;
    IWorker* worker;
};

TEST_F(ExitWorker_tests, nothing_done_yet) {
    ExitWorker exitWorker{ipcMock};
    worker = &exitWorker;
    EXPECT_FALSE(worker->isWorking());
}

TEST_F(ExitWorker_tests, startWorking_first_time) {
    ExitWorker exitWorker{ipcMock};
    worker = &exitWorker;
    EXPECT_CALL(ipcMock, open()).Times(1);
    worker->startWorking();
    EXPECT_TRUE(worker->isWorking());
}

TEST_F(ExitWorker_tests, startWorking_double_times) {
    ExitWorker exitWorker{ipcMock};
    worker = &exitWorker;
    EXPECT_CALL(ipcMock, open()).Times(1);
    worker->startWorking();
    worker->startWorking();
    EXPECT_TRUE(worker->isWorking());
}

TEST_F(ExitWorker_tests, processData_success) {
    ExitWorker exitWorker{ipcMock};
    worker = &exitWorker;

    EXPECT_CALL(ipcMock, open()).Times(1);
    EXPECT_CALL(ipcMock, read()).WillOnce(Return(""));
    EXPECT_CALL(ipcMock, write(exit_msg)).Times(1);

    worker->startWorking();
    worker->processData();
}

TEST_F(ExitWorker_tests, processData_not_started_yet) {
    ExitWorker exitWorker{ipcMock};
    worker = &exitWorker;

    EXPECT_CALL(ipcMock, read()).Times(0);
    EXPECT_CALL(ipcMock, write(exit_msg)).Times(0);

    worker->processData();
}

TEST_F(ExitWorker_tests, stopWorking_success) {
    ExitWorker exitWorker{ipcMock};
    worker = &exitWorker;

    EXPECT_CALL(ipcMock, open()).Times(1);
    EXPECT_CALL(ipcMock, close()).Times(1);

    worker->startWorking();
    EXPECT_TRUE(worker->isWorking());
    worker->stopWorking();
    EXPECT_FALSE(worker->isWorking());
}

TEST_F(ExitWorker_tests, stopWorking_double_times) {
    ExitWorker exitWorker{ipcMock};
    worker = &exitWorker;

    EXPECT_CALL(ipcMock, open()).Times(1);
    EXPECT_CALL(ipcMock, close()).Times(1);

    worker->startWorking();
    EXPECT_TRUE(worker->isWorking());
    worker->stopWorking();
    worker->stopWorking();
    EXPECT_FALSE(worker->isWorking());
}

TEST_F(ExitWorker_tests, stopWorking_not_started_yet) {
    ExitWorker exitWorker{ipcMock};
    worker = &exitWorker;

    EXPECT_CALL(ipcMock, close()).Times(0);

    worker->stopWorking();
    EXPECT_FALSE(worker->isWorking());
}

TEST_F(ExitWorker_tests, stopWorking_when_exit_received) {
    ExitWorker exitWorker{ipcMock};
    worker = &exitWorker;

    EXPECT_CALL(ipcMock, open()).Times(1);
    EXPECT_CALL(ipcMock, read()).WillOnce(Return(exit_msg));
    EXPECT_CALL(ipcMock, close()).Times(1);

    worker->startWorking();
    EXPECT_TRUE(worker->isWorking());
    worker->processData();
    EXPECT_FALSE(worker->isWorking());
}

} // anonymous