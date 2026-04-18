#include <gtest/gtest.h>

#include <mocks/ipc/IpcMock.h>

#include <proc_managers/workers/PressureWorker.h>
#include <proc_managers/workers/WeatherData.h>

namespace {

using namespace testing;
using namespace mw::mocks;
using namespace mw::proc_managers::workers;

constexpr const std::size_t bufferSize = 2;

class PressureWorker_tests : public Test {
public:
    ~PressureWorker_tests() = default;

protected:
    void TearDown() {
        worker = nullptr;
    }

    IpcMock ipcMock;
    IWorker* worker;
};

TEST_F(PressureWorker_tests, nothing_done_yet) {
    PressureWorker pressureWorker{ipcMock, bufferSize};
    worker = &pressureWorker;
    EXPECT_FALSE(worker->isWorking());
}

TEST_F(PressureWorker_tests, startWorking_first_time) {
    PressureWorker pressureWorker{ipcMock, bufferSize};
    worker = &pressureWorker;
    EXPECT_CALL(ipcMock, open()).Times(1);
    worker->startWorking();
    EXPECT_TRUE(worker->isWorking());
}

TEST_F(PressureWorker_tests, startWorking_double_times) {
    PressureWorker pressureWorker{ipcMock, bufferSize};
    worker = &pressureWorker;
    EXPECT_CALL(ipcMock, open()).Times(1);
    worker->startWorking();
    worker->startWorking();
    EXPECT_TRUE(worker->isWorking());
}

TEST_F(PressureWorker_tests, processData_success) {
    PressureWorker pressureWorker{ipcMock, bufferSize};
    worker = &pressureWorker;
    WeatherData data;

    data.setTemperature(-17.4);
    data.setPressure(1007.4);

    EXPECT_CALL(ipcMock, open()).Times(1);
    EXPECT_CALL(ipcMock, read()).WillOnce(Return(data.serialize()));

    worker->startWorking();
    worker->processData();
}

TEST_F(PressureWorker_tests, processData_not_started_yet) {
    PressureWorker pressureWorker{ipcMock, bufferSize};
    worker = &pressureWorker;

    EXPECT_CALL(ipcMock, read()).Times(0);

    worker->processData();
}

TEST_F(PressureWorker_tests, stopWorking_success) {
    PressureWorker pressureWorker{ipcMock, bufferSize};
    worker = &pressureWorker;

    EXPECT_CALL(ipcMock, open()).Times(1);
    EXPECT_CALL(ipcMock, close()).Times(1);

    worker->startWorking();
    EXPECT_TRUE(worker->isWorking());
    worker->stopWorking();
    EXPECT_FALSE(worker->isWorking());
}

TEST_F(PressureWorker_tests, stopWorking_double_times) {
    PressureWorker pressureWorker{ipcMock, bufferSize};
    worker = &pressureWorker;

    EXPECT_CALL(ipcMock, open()).Times(1);
    EXPECT_CALL(ipcMock, close()).Times(1);

    worker->startWorking();
    EXPECT_TRUE(worker->isWorking());
    worker->stopWorking();
    worker->stopWorking();
    EXPECT_FALSE(worker->isWorking());
}

TEST_F(PressureWorker_tests, stopWorking_not_started_yet) {
    PressureWorker pressureWorker{ipcMock, bufferSize};
    worker = &pressureWorker;

    EXPECT_CALL(ipcMock, close()).Times(0);

    worker->stopWorking();
    EXPECT_FALSE(worker->isWorking());
}

} // anonymous