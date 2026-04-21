#include <gtest/gtest.h>

#include <mocks/ipc/IpcMock.h>

#include <proc_managers/workers/WeatherWorker.h>

namespace {

using namespace testing;
using namespace mw::mocks;
using namespace mw::proc_managers::workers;

class WeatherWorker_tests : public Test {
public:
    ~WeatherWorker_tests() = default;

protected:
    void TearDown() {
        worker = nullptr;
    }

    StrictMock<IpcMock> ipcMock;
    IWorker* worker;
};

TEST_F(WeatherWorker_tests, nothing_done_yet) {
    WeatherWorker weatherWorker{ipcMock};
    worker = &weatherWorker;
    EXPECT_FALSE(worker->isWorking());
}

TEST_F(WeatherWorker_tests, startWorking_first_time) {
    WeatherWorker weatherWorker{ipcMock};
    worker = &weatherWorker;
    EXPECT_CALL(ipcMock, open()).Times(1);
    worker->startWorking();
    EXPECT_TRUE(worker->isWorking());
}

TEST_F(WeatherWorker_tests, startWorking_double_times) {
    WeatherWorker weatherWorker{ipcMock};
    worker = &weatherWorker;
    EXPECT_CALL(ipcMock, open()).Times(1);
    worker->startWorking();
    worker->startWorking();
    EXPECT_TRUE(worker->isWorking());
}

TEST_F(WeatherWorker_tests, processData_success) {
    WeatherWorker weatherWorker{ipcMock};
    worker = &weatherWorker;

    EXPECT_CALL(ipcMock, open()).Times(1);
    EXPECT_CALL(ipcMock, write(_)).Times(1);

    worker->startWorking();
    worker->processData();
}

TEST_F(WeatherWorker_tests, processData_not_started) {
    WeatherWorker weatherWorker{ipcMock};
    worker = &weatherWorker;

    EXPECT_CALL(ipcMock, write(_)).Times(0);
    worker->processData();
}

TEST_F(WeatherWorker_tests, stopWorking_success) {
    WeatherWorker weatherWorker{ipcMock};
    worker = &weatherWorker;

    EXPECT_CALL(ipcMock, open()).Times(1);
    EXPECT_CALL(ipcMock, close()).Times(1);

    worker->startWorking();
    EXPECT_TRUE(worker->isWorking());
    worker->stopWorking();
    EXPECT_FALSE(worker->isWorking());
}

TEST_F(WeatherWorker_tests, stopWorking_double_times) {
    WeatherWorker weatherWorker{ipcMock};
    worker = &weatherWorker;

    EXPECT_CALL(ipcMock, open()).Times(1);
    EXPECT_CALL(ipcMock, close()).Times(1);

    worker->startWorking();
    EXPECT_TRUE(worker->isWorking());
    worker->stopWorking();
    worker->stopWorking();
    EXPECT_FALSE(worker->isWorking());
}

TEST_F(WeatherWorker_tests, stopWorking_not_started_yet) {
    WeatherWorker weatherWorker{ipcMock};
    worker = &weatherWorker;

    EXPECT_CALL(ipcMock, close()).Times(0);

    worker->stopWorking();
    EXPECT_FALSE(worker->isWorking());
}

} // anonymous