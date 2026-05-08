#include <gtest/gtest.h>

#include <mocks/ipc/IpcMock.h>

#include <proc_managers/workers/GnuplotWorker.h>

namespace {

using namespace testing;
using namespace mw::mocks;
using namespace mw::proc_managers::workers;

class GnuplotWorker_tests : public Test {
public:
    ~GnuplotWorker_tests() = default;

protected:
    void SetUp() {
        EXPECT_CALL(ipcPipeMock, write(_)).WillRepeatedly(Return(true));
    }

    void TearDown() {
        worker = nullptr;
    }

    StrictMock<IpcMock> ipcMemMock;
    StrictMock<IpcMock> ipcPipeMock;
    IWorker* worker;
};

TEST_F(GnuplotWorker_tests, nothing_done_yet) {
    GnuplotWorker gnuplotWorker{ipcMemMock, ipcPipeMock};
    worker = &gnuplotWorker;
    EXPECT_FALSE(worker->isWorking());
}

TEST_F(GnuplotWorker_tests, startWorking_first_time) {
    GnuplotWorker gnuplotWorker{ipcMemMock, ipcPipeMock};
    worker = &gnuplotWorker;

    EXPECT_CALL(ipcMemMock, open()).Times(1);
    EXPECT_CALL(ipcPipeMock, open()).Times(1);

    worker->startWorking();
    EXPECT_TRUE(worker->isWorking());
}

TEST_F(GnuplotWorker_tests, startWorking_double_times) {
    GnuplotWorker gnuplotWorker{ipcMemMock, ipcPipeMock};
    worker = &gnuplotWorker;

    EXPECT_CALL(ipcMemMock, open()).Times(1);
    EXPECT_CALL(ipcPipeMock, open()).Times(1);

    worker->startWorking();
    worker->startWorking();
    EXPECT_TRUE(worker->isWorking());
}

TEST_F(GnuplotWorker_tests, processData_success) {
    GnuplotWorker gnuplotWorker{ipcMemMock, ipcPipeMock};
    worker = &gnuplotWorker;

    EXPECT_CALL(ipcMemMock, open()).Times(1);
    EXPECT_CALL(ipcPipeMock, open()).Times(1);
    EXPECT_CALL(ipcMemMock, read()).WillOnce(Return("test data"));
    EXPECT_CALL(ipcPipeMock, write(_)).WillOnce(Return(true));


}

} // anonymous