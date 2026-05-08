#include <gtest/gtest.h>

#include <mocks/ipc/IpcMock.h>

#include <proc_managers/workers/GnuplotWorker.h>
#include <helpers/GnuplotCommander.h>

namespace {

using namespace testing;
using namespace mw::mocks;
using namespace mw::proc_managers::workers;
using namespace mw::helpers;

constexpr const char* DATA_FILE_NAME = "data_file.dat";

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
    GnuplotWorker gnuplotWorker{ipcMemMock, ipcPipeMock, DATA_FILE_NAME};
    worker = &gnuplotWorker;
    EXPECT_FALSE(worker->isWorking());
}

TEST_F(GnuplotWorker_tests, startWorking_first_time) {
    GnuplotWorker gnuplotWorker{ipcMemMock, ipcPipeMock, DATA_FILE_NAME};
    worker = &gnuplotWorker;

    EXPECT_CALL(ipcMemMock, open()).Times(1);
    EXPECT_CALL(ipcPipeMock, open()).Times(1);

    worker->startWorking();
    EXPECT_TRUE(worker->isWorking());
}

TEST_F(GnuplotWorker_tests, startWorking_double_times) {
    GnuplotWorker gnuplotWorker{ipcMemMock, ipcPipeMock, DATA_FILE_NAME};
    worker = &gnuplotWorker;

    EXPECT_CALL(ipcMemMock, open()).Times(1);
    EXPECT_CALL(ipcPipeMock, open()).Times(1);

    worker->startWorking();
    worker->startWorking();
    EXPECT_TRUE(worker->isWorking());
}

TEST_F(GnuplotWorker_tests, processData_success) {
    GnuplotWorker gnuplotWorker{ipcMemMock, ipcPipeMock, DATA_FILE_NAME};
    worker = &gnuplotWorker;
    const std::string cmd = GnuplotCommander::plotPoints(DATA_FILE_NAME);

    EXPECT_CALL(ipcMemMock, open()).Times(1);
    EXPECT_CALL(ipcPipeMock, open()).Times(1);
    EXPECT_CALL(ipcMemMock, read()).WillOnce(Return("test data"));
    EXPECT_CALL(ipcPipeMock, write(cmd)).WillOnce(Return(true));

    worker->startWorking();
    worker->processData();
}

TEST_F(GnuplotWorker_tests, processData_not_started_yet) {
    GnuplotWorker gnuplotWorker{ipcMemMock, ipcPipeMock, DATA_FILE_NAME};
    worker = &gnuplotWorker;
    const std::string cmd = GnuplotCommander::plotPoints(DATA_FILE_NAME);

    EXPECT_CALL(ipcMemMock, read()).Times(0);
    EXPECT_CALL(ipcPipeMock, write(cmd)).Times(0);

    worker->processData();
}

TEST_F(GnuplotWorker_tests, stopWorking_success) {
    GnuplotWorker gnuplotWorker{ipcMemMock, ipcPipeMock, DATA_FILE_NAME};
    worker = &gnuplotWorker;

    EXPECT_CALL(ipcMemMock, open()).Times(1);
    EXPECT_CALL(ipcPipeMock, open()).Times(1);
    EXPECT_CALL(ipcMemMock, close()).Times(1);
    EXPECT_CALL(ipcPipeMock, close()).Times(1);

    worker->startWorking();
    EXPECT_TRUE(worker->isWorking());
    worker->stopWorking();
    EXPECT_FALSE(worker->isWorking());
}

TEST_F(GnuplotWorker_tests, stopWorking_not_started_yet) {
    GnuplotWorker gnuplotWorker{ipcMemMock, ipcPipeMock, DATA_FILE_NAME};
    worker = &gnuplotWorker;

    EXPECT_CALL(ipcMemMock, close()).Times(0);
    EXPECT_CALL(ipcPipeMock, close()).Times(0);

    worker->stopWorking();
    EXPECT_FALSE(worker->isWorking());
}

TEST_F(GnuplotWorker_tests, stopWorking_when_exit_received) {
    GnuplotWorker gnuplotWorker{ipcMemMock, ipcPipeMock, DATA_FILE_NAME};
    worker = &gnuplotWorker;
    const std::string cmd = GnuplotCommander::plotPoints(DATA_FILE_NAME);

    EXPECT_CALL(ipcMemMock, open()).Times(1);
    EXPECT_CALL(ipcPipeMock, open()).Times(1);
    EXPECT_CALL(ipcMemMock, read()).WillOnce(Return("exit"));
    EXPECT_CALL(ipcPipeMock, write(cmd)).Times(0);
    EXPECT_CALL(ipcMemMock, close()).Times(1);
    EXPECT_CALL(ipcPipeMock, close()).Times(1);

    worker->startWorking();
    EXPECT_TRUE(worker->isWorking());
    worker->processData();
    EXPECT_FALSE(worker->isWorking());
}

} // anonymous