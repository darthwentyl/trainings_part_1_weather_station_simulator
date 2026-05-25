#include <gtest/gtest.h>

#include <mocks/ipc/IpcMock.h>

#include <proc_managers/workers/UserCmdWorker.h>

namespace {

using namespace testing;
using namespace mw::mocks;
using namespace mw::proc_managers::workers;

class UserCmdWorker_tests : public Test {
public:
    ~UserCmdWorker_tests() = default;

protected:
    void TearDown() override {
        worker = nullptr;
    }

    StrictMock<IpcMock> ipcMemMock;
    StrictMock<IpcMock> ipcSocketMock;
    IWorker* worker;
};

TEST_F(UserCmdWorker_tests, nothing_done_yet) {
    UserCmdWorker userCmdWorker{ipcMemMock, ipcSocketMock};
    worker = &userCmdWorker;
    EXPECT_FALSE(worker->isWorking());
}

TEST_F(UserCmdWorker_tests, startWorking_first_time) {
    UserCmdWorker userCmdWorker{ipcMemMock, ipcSocketMock};
    worker = &userCmdWorker;

    EXPECT_CALL(ipcMemMock, open()).Times(1);
    EXPECT_CALL(ipcSocketMock, open()).Times(1);

    worker->startWorking();
    EXPECT_TRUE(worker->isWorking());
}

TEST_F(UserCmdWorker_tests, startWorking_double_times) {
    UserCmdWorker userCmdWorker{ipcMemMock, ipcSocketMock};
    worker = &userCmdWorker;

    EXPECT_CALL(ipcMemMock, open()).Times(1);
    EXPECT_CALL(ipcSocketMock, open()).Times(1);

    worker->startWorking();
    worker->startWorking();
    EXPECT_TRUE(worker->isWorking());
}

TEST_F(UserCmdWorker_tests, stopWorking_success) {
    UserCmdWorker userCmdWorker{ipcMemMock, ipcSocketMock};
    worker = &userCmdWorker;

    EXPECT_CALL(ipcMemMock, open()).Times(1);
    EXPECT_CALL(ipcSocketMock, open()).Times(1);
    EXPECT_CALL(ipcMemMock, close()).Times(1);
    EXPECT_CALL(ipcSocketMock, close()).Times(1);

    worker->startWorking();
    EXPECT_TRUE(worker->isWorking());
    worker->stopWorking();
    EXPECT_FALSE(worker->isWorking());
}

TEST_F(UserCmdWorker_tests, stopWorking_not_started_yet) {
    UserCmdWorker userCmdWorker{ipcMemMock, ipcSocketMock};
    worker = &userCmdWorker;

    EXPECT_CALL(ipcMemMock, close()).Times(0);
    EXPECT_CALL(ipcSocketMock, close()).Times(0);

    worker->stopWorking();
    EXPECT_FALSE(worker->isWorking());
}

} // anonymous