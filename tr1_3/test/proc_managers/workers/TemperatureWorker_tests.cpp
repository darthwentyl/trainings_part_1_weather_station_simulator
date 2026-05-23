#include <gtest/gtest.h>

#include <mocks/ipc/IpcMock.h>
#include <mocks/StdLibStaticMock.h>

#include <dlfcn.h>
#include <unistd.h>
#include <cstddef>
#include <sys/types.h>

#include <proc_managers/workers/TemperatureWorker.h>
#include <proc_managers/workers/WeatherData.h>

namespace {

using namespace testing;
using namespace mw::mocks;
using namespace mw::proc_managers::workers;

constexpr const std::size_t bufferSize = 2;
constexpr const char* dataFile = "data_file.dat";

class TemperatureWorker_tests : public Test {
public:
    ~TemperatureWorker_tests() = default;

protected:
    void SetUp() override {
        os_read = (ssize_t(*)(int, void*, size_t))dlsym(RTLD_NEXT, "read");
        os_write = (ssize_t(*)(int, const void*, size_t))dlsym(RTLD_NEXT, "write");
        auto& stdLib = StdLibStaticMock::get();
        EXPECT_CALL(stdLib, read(_, _, _)).WillRepeatedly(Invoke(os_read));
        EXPECT_CALL(stdLib, write(_, _, _)).WillRepeatedly(Invoke(os_write));
    }

    void  TearDown() override {
        worker = nullptr;
    }

    StrictMock<IpcMock> ipcMock;
    IWorker* worker;
    std::function<ssize_t(int, void*, size_t)> os_read;
    std::function<ssize_t(int, const void*, size_t)> os_write;
};

TEST_F(TemperatureWorker_tests, nothing_done_yet) {
    TemperatureWorker temperatureWorker{ipcMock, dataFile, bufferSize};
    worker = &temperatureWorker;
    EXPECT_FALSE(worker->isWorking());
}

TEST_F(TemperatureWorker_tests, startWorking_first_time) {
    TemperatureWorker temperatureWorker{ipcMock, dataFile, bufferSize};
    worker = &temperatureWorker;
    EXPECT_CALL(ipcMock, open()).Times(1);
    worker->startWorking();
    EXPECT_TRUE(worker->isWorking());
}

TEST_F(TemperatureWorker_tests, startWorking_double_times) {
    TemperatureWorker temperatureWorker{ipcMock, dataFile, bufferSize};
    worker = &temperatureWorker;
    EXPECT_CALL(ipcMock, open()).Times(1);
    worker->startWorking();
    worker->startWorking();
    EXPECT_TRUE(worker->isWorking());
}

TEST_F(TemperatureWorker_tests, processData_success) {
    TemperatureWorker temperatureWorker{ipcMock, dataFile, bufferSize};
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
    TemperatureWorker temperatureWorker{ipcMock, dataFile, bufferSize};
    worker = &temperatureWorker;

    EXPECT_CALL(ipcMock, read()).Times(0);

    worker->processData();
}

TEST_F(TemperatureWorker_tests, stopWorking_success) {
    TemperatureWorker temperatureWorker{ipcMock, dataFile, bufferSize};
    worker = &temperatureWorker;

    EXPECT_CALL(ipcMock, open()).Times(1);
    EXPECT_CALL(ipcMock, close()).Times(1);

    worker->startWorking();
    EXPECT_TRUE(worker->isWorking());
    worker->stopWorking();
    EXPECT_FALSE(worker->isWorking());
}

TEST_F(TemperatureWorker_tests, stopWorking_double_times) {
    TemperatureWorker temperatureWorker{ipcMock, dataFile, bufferSize};
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
    TemperatureWorker temperatureWorker{ipcMock, dataFile, bufferSize};
    worker = &temperatureWorker;

    EXPECT_CALL(ipcMock, close()).Times(0);

    worker->stopWorking();
    EXPECT_FALSE(worker->isWorking());
}

TEST_F(TemperatureWorker_tests, stopWorking_when_exit_received) {
    TemperatureWorker temperatureWorker{ipcMock, dataFile, bufferSize};
    worker = &temperatureWorker;

    EXPECT_CALL(ipcMock, open()).Times(1);
    EXPECT_CALL(ipcMock, read()).WillOnce(Return("exit"));
    EXPECT_CALL(ipcMock, close()).Times(1);

    worker->startWorking();
    EXPECT_TRUE(worker->isWorking());
    worker->processData();
    EXPECT_FALSE(worker->isWorking());
}

} // anonymous