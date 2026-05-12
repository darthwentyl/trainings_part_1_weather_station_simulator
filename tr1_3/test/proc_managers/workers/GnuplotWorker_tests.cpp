#include <gtest/gtest.h>

#include <mocks/ipc/IpcMock.h>

#include <proc_managers/workers/GnuplotWorker.h>

#include <helpers/GnuplotCommander.h>
#include <helpers/GnuplotDescriptionBuilder.h>

#include <fstream>
#include <filesystem>

namespace {

using namespace testing;
using namespace mw::mocks;
using namespace mw::proc_managers::workers;
using namespace mw::helpers;

namespace fs = std::filesystem;

constexpr const std::size_t WIDTH = 800;
constexpr const std::size_t HEIGHT = 600;
constexpr const char* TITLE = "test title";
constexpr const char* X_LABEL = "test x label";
constexpr const char* Y_LABEL = "test y label";
constexpr const char* LEGEND = "test legend";
constexpr const char* DATA_FILE = "test data file";

class GnuplotWorker_tests : public Test {
public:
    ~GnuplotWorker_tests() = default;

protected:
    void SetUp() {
        description = GnuplotDescriptionBuilder()
            .width(WIDTH)
            .height(HEIGHT)
            .title(TITLE)
            .xLabel(X_LABEL)
            .yLabel(Y_LABEL)
            .legend(LEGEND)
            .dataFile(DATA_FILE)
            .grid(true)
            .build();

        EXPECT_CALL(ipcPipeMock, write(_)).WillRepeatedly(Return(true));
    }

    void TearDown() {
        worker = nullptr;
        if (fs::exists(DATA_FILE)) {
            std::remove(DATA_FILE);
        }
    }

    StrictMock<IpcMock> ipcMemMock;
    StrictMock<IpcMock> ipcPipeMock;
    GnuplotDescription description;
    IWorker* worker;
};

TEST_F(GnuplotWorker_tests, nothing_done_yet) {
    GnuplotWorker gnuplotWorker{ipcMemMock, ipcPipeMock, description};
    worker = &gnuplotWorker;
    EXPECT_FALSE(worker->isWorking());
}

TEST_F(GnuplotWorker_tests, startWorking_first_time) {
    GnuplotWorker gnuplotWorker{ipcMemMock, ipcPipeMock, description};
    worker = &gnuplotWorker;

    EXPECT_CALL(ipcMemMock, open()).Times(1);
    EXPECT_CALL(ipcPipeMock, open()).Times(1);

    worker->startWorking();
    EXPECT_TRUE(worker->isWorking());
}

TEST_F(GnuplotWorker_tests, startWorking_double_times) {
    GnuplotWorker gnuplotWorker{ipcMemMock, ipcPipeMock, description};
    worker = &gnuplotWorker;

    EXPECT_CALL(ipcMemMock, open()).Times(1);
    EXPECT_CALL(ipcPipeMock, open()).Times(1);

    worker->startWorking();
    worker->startWorking();
    EXPECT_TRUE(worker->isWorking());
}

TEST_F(GnuplotWorker_tests, processData_success) {
    {
        std::ofstream dataFile(DATA_FILE);
    }

    GnuplotWorker gnuplotWorker{ipcMemMock, ipcPipeMock, description};
    worker = &gnuplotWorker;
    const std::string cmd =
        GnuplotCommander::plotPoints(description.getDataFile(), description.getLegend());

    EXPECT_CALL(ipcMemMock, open()).Times(1);
    EXPECT_CALL(ipcPipeMock, open()).Times(1);
    EXPECT_CALL(ipcMemMock, read()).WillOnce(Return("test data"));
    EXPECT_CALL(ipcPipeMock, write(cmd)).WillOnce(Return(true));

    worker->startWorking();
    worker->processData();
}

TEST_F(GnuplotWorker_tests, processData_not_started_yet) {
    GnuplotWorker gnuplotWorker{ipcMemMock, ipcPipeMock, description};
    worker = &gnuplotWorker;
    const std::string cmd =
        GnuplotCommander::plotPoints(description.getDataFile(), description.getLegend());

    EXPECT_CALL(ipcMemMock, read()).Times(0);
    EXPECT_CALL(ipcPipeMock, write(cmd)).Times(0);

    worker->processData();
}

TEST_F(GnuplotWorker_tests, stopWorking_success) {
    GnuplotWorker gnuplotWorker{ipcMemMock, ipcPipeMock, description};
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
    GnuplotWorker gnuplotWorker{ipcMemMock, ipcPipeMock, description};
    worker = &gnuplotWorker;

    EXPECT_CALL(ipcMemMock, close()).Times(0);
    EXPECT_CALL(ipcPipeMock, close()).Times(0);

    worker->stopWorking();
    EXPECT_FALSE(worker->isWorking());
}

TEST_F(GnuplotWorker_tests, stopWorking_when_exit_received) {
    GnuplotWorker gnuplotWorker{ipcMemMock, ipcPipeMock, description};
    worker = &gnuplotWorker;
    const std::string cmd =
        GnuplotCommander::plotPoints(description.getDataFile(), description.getLegend());

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