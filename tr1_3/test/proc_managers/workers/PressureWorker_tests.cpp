#include <gtest/gtest.h>

#include <mocks/ipc/IpcMock.h>
#include <mocks/StdLibStaticMock.h>

#include <dlfcn.h>
#include <unistd.h>
#include <cstddef>
#include <sys/types.h>

#include <proc_managers/workers/PressureWorker.h>
#include <proc_managers/workers/WeatherData.h>

namespace {

using namespace testing;
using namespace mw::mocks;
using namespace mw::proc_managers::workers;

constexpr const std::size_t bufferSize = 2;
constexpr const char* dataFile = "data_file.dat";

class PressureWorker_tests : public Test {
public:
    ~PressureWorker_tests() = default;

protected:
    void SetUp() override {
        os_read = (ssize_t(*)(int, void*, size_t))dlsym(RTLD_NEXT, "read");
        os_write = (ssize_t(*)(int, const void*, size_t))dlsym(RTLD_NEXT, "write");
        auto& stdLib = StdLibStaticMock::get();
        EXPECT_CALL(stdLib, read(_, _, _)).WillRepeatedly(Invoke(os_read));
        EXPECT_CALL(stdLib, write(_, _, _)).WillRepeatedly(Invoke(os_write));
    }

    void TearDown() override {
        worker = nullptr;
    }

    StrictMock<IpcMock> ipcMock;
    IWorker* worker;
    std::function<ssize_t(int, void*, size_t)> os_read;
    std::function<ssize_t(int, const void*, size_t)> os_write;
};

TEST_F(PressureWorker_tests, nothing_done_yet) {
    PressureWorker pressureWorker{ipcMock, dataFile, bufferSize};
    worker = &pressureWorker;
    EXPECT_FALSE(worker->isWorking());
}

TEST_F(PressureWorker_tests, startWorking_first_time) {
    PressureWorker pressureWorker{ipcMock, dataFile, bufferSize};
    worker = &pressureWorker;
    EXPECT_CALL(ipcMock, open()).Times(1);
    worker->startWorking();
    EXPECT_TRUE(worker->isWorking());
}

TEST_F(PressureWorker_tests, startWorking_double_times) {
    PressureWorker pressureWorker{ipcMock, dataFile, bufferSize};
    worker = &pressureWorker;
    EXPECT_CALL(ipcMock, open()).Times(1);
    worker->startWorking();
    worker->startWorking();
    EXPECT_TRUE(worker->isWorking());
}

TEST_F(PressureWorker_tests, processData_success) {
    PressureWorker pressureWorker{ipcMock, dataFile, bufferSize};
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
    PressureWorker pressureWorker{ipcMock, dataFile, bufferSize};
    worker = &pressureWorker;

    EXPECT_CALL(ipcMock, read()).Times(0);

    worker->processData();
}

TEST_F(PressureWorker_tests, stopWorking_success) {
    PressureWorker pressureWorker{ipcMock, dataFile, bufferSize};
    worker = &pressureWorker;

    EXPECT_CALL(ipcMock, open()).Times(1);
    EXPECT_CALL(ipcMock, close()).Times(1);

    worker->startWorking();
    EXPECT_TRUE(worker->isWorking());
    worker->stopWorking();
    EXPECT_FALSE(worker->isWorking());
}

TEST_F(PressureWorker_tests, stopWorking_double_times) {
    PressureWorker pressureWorker{ipcMock, dataFile, bufferSize};
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
    PressureWorker pressureWorker{ipcMock, dataFile, bufferSize};
    worker = &pressureWorker;

    EXPECT_CALL(ipcMock, close()).Times(0);

    worker->stopWorking();
    EXPECT_FALSE(worker->isWorking());
}

TEST_F(PressureWorker_tests, stopWorking_when_exit_received) {
    PressureWorker pressureWorker{ipcMock, dataFile, bufferSize};
    worker = &pressureWorker;

    EXPECT_CALL(ipcMock, open()).Times(1);
    EXPECT_CALL(ipcMock, read()).WillOnce(Return("exit"));
    EXPECT_CALL(ipcMock, close()).Times(1);

    worker->startWorking();
    EXPECT_TRUE(worker->isWorking());
    worker->processData();
    EXPECT_FALSE(worker->isWorking());
}

} // anonymous