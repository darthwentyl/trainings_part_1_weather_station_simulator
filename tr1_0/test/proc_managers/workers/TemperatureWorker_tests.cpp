#include <gtest/gtest.h>

#include <mocks/ipc/IpcMock.h>

#include <proc_managers/workers/TemperatureWorker.h>
#include <proc_managers/workers/WeatherData.h>

namespace {

using namespace testing;
using namespace mocks;
using namespace mw::proc_managers::workers;

constexpr const std::size_t bufferSize = 2;

class TemperatureWorker_tests : public Test {
public:
    ~TemperatureWorker_tests() = default;

protected:
    void  TearDown() {
        worker = nullptr;
    }

    IpcMock ipcMock;
    IWorker* worker;
};

TEST_F(TemperatureWorker_tests, nothing_done_yet) {
    TemperatureWorker temperatureWorker{ipcMock, bufferSize};
    worker = &temperatureWorker;
    EXPECT_FALSE(worker->isWorking());
}

TEST_F(TemperatureWorker_tests, startWorking_first_time) {
    TemperatureWorker temperatureWorker{ipcMock, bufferSize};
    worker = &temperatureWorker;
    EXPECT_CALL(ipcMock, open()).Times(1);
    worker->startWorking();
    EXPECT_TRUE(worker->isWorking());
}

TEST_F(TemperatureWorker_tests, startWorking_double_times) {
    TemperatureWorker temperatureWorker{ipcMock, bufferSize};
    worker = &temperatureWorker;
    EXPECT_CALL(ipcMock, open()).Times(1);
    worker->startWorking();
    worker->startWorking();
    EXPECT_TRUE(worker->isWorking());
}

TEST_F(TemperatureWorker_tests, processData_success) {
    TemperatureWorker temperatureWorker{ipcMock, bufferSize};
    worker = &temperatureWorker;
    WeatherData data;

    data.setTemperature(17.3);
    data.setPressure(1008.1);

    EXPECT_CALL(ipcMock, open()).Times(1);
    EXPECT_CALL(ipcMock, read()).WillOnce(Return(data.serialize()));

    worker->startWorking();
    worker->processData();
}

TEST_F(TemperatureWorker_tests, processData_not_started_yet) {
    TemperatureWorker temperatureWorker{ipcMock, bufferSize};
    worker = &temperatureWorker;

    EXPECT_CALL(ipcMock, read()).Times(0);

    worker->processData();
}

TEST_F(TemperatureWorker_tests, stopWorking_success) {
    TemperatureWorker temperatureWorker{ipcMock, bufferSize};
    worker = &temperatureWorker;

    EXPECT_CALL(ipcMock, open()).Times(1);
    EXPECT_CALL(ipcMock, close()).Times(1);

    worker->startWorking();
    EXPECT_TRUE(worker->isWorking());
    worker->stopWorking();
    EXPECT_FALSE(worker->isWorking());
}

TEST_F(TemperatureWorker_tests, stopWorking_double_times) {
    TemperatureWorker temperatureWorker{ipcMock, bufferSize};
    worker = &temperatureWorker;

    EXPECT_CALL(ipcMock, open()).Times(1);
    EXPECT_CALL(ipcMock, close()).Times(1);

    worker->startWorking();
    EXPECT_TRUE(worker->isWorking());
    worker->stopWorking();
    worker->stopWorking();
    EXPECT_FALSE(worker->isWorking());
}

TEST_F(TemperatureWorker_tests, stopWorking_not_started_yet) {
    TemperatureWorker temperatureWorker{ipcMock, bufferSize};
    worker = &temperatureWorker;

    EXPECT_CALL(ipcMock, close()).Times(0);

    worker->stopWorking();
}

} // anonymous