#include <gtest/gtest.h>

#include <mocks/ipc/IpcMock.h>

#include <proc_managers/workers/UserCmdWorker.h>

#include <chrono>

namespace {

using namespace testing;
using namespace mw::mocks;
using namespace mw::proc_managers::workers;
using namespace std::chrono_literals;

constexpr const std::size_t BUFFER_SIZE = 2;

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
    UserCmdWorker userCmdWorker{ipcMemMock, ipcSocketMock, BUFFER_SIZE};
    worker = &userCmdWorker;
    EXPECT_FALSE(worker->isWorking());
}

TEST_F(UserCmdWorker_tests, startWorking_first_time) {
    UserCmdWorker userCmdWorker{ipcMemMock, ipcSocketMock, BUFFER_SIZE};
    worker = &userCmdWorker;

    EXPECT_CALL(ipcMemMock, open()).Times(1);
    EXPECT_CALL(ipcSocketMock, open()).Times(1);
    EXPECT_CALL(ipcSocketMock, read()).WillOnce(Invoke(
        []() {
            std::this_thread::sleep_for(25ms);
            return std::string{};
        }
    ));
    EXPECT_CALL(ipcSocketMock, close()).Times(1);

    worker->startWorking();
    EXPECT_TRUE(worker->isWorking());
}

TEST_F(UserCmdWorker_tests, startWorking_double_times) {
    UserCmdWorker userCmdWorker{ipcMemMock, ipcSocketMock, BUFFER_SIZE};
    worker = &userCmdWorker;

    EXPECT_CALL(ipcMemMock, open()).Times(1);
    EXPECT_CALL(ipcSocketMock, open()).Times(1);
    EXPECT_CALL(ipcSocketMock, read()).WillOnce(Invoke(
        []() {
            std::this_thread::sleep_for(25ms);
            return std::string{};
        }
    ));
    EXPECT_CALL(ipcSocketMock, close()).Times(1);

    worker->startWorking();
    worker->startWorking();
    EXPECT_TRUE(worker->isWorking());
}

TEST_F(UserCmdWorker_tests, stopWorking_success) {
    UserCmdWorker userCmdWorker{ipcMemMock, ipcSocketMock, BUFFER_SIZE};
    worker = &userCmdWorker;

    EXPECT_CALL(ipcMemMock, open()).Times(1);
    EXPECT_CALL(ipcSocketMock, open()).Times(1);
    EXPECT_CALL(ipcSocketMock, read()).WillOnce(Invoke(
        []() {
            std::this_thread::sleep_for(25ms);
            return std::string{};
        }
    ));
    EXPECT_CALL(ipcMemMock, close()).Times(1);
    EXPECT_CALL(ipcSocketMock, close()).Times(1);

    worker->startWorking();
    EXPECT_TRUE(worker->isWorking());
    worker->stopWorking();
    EXPECT_FALSE(worker->isWorking());
}

TEST_F(UserCmdWorker_tests, stopWorking_not_started_yet) {
    UserCmdWorker userCmdWorker{ipcMemMock, ipcSocketMock, BUFFER_SIZE};
    worker = &userCmdWorker;

    EXPECT_CALL(ipcMemMock, close()).Times(0);
    EXPECT_CALL(ipcSocketMock, close()).Times(0);

    worker->stopWorking();
    EXPECT_FALSE(worker->isWorking());
}

TEST_F(UserCmdWorker_tests, processData_not_started_yet) {
    UserCmdWorker userCmdWorker{ipcMemMock, ipcSocketMock, BUFFER_SIZE};
    worker = &userCmdWorker;

    EXPECT_CALL(ipcMemMock, read()).Times(0);

    worker->processData();
    EXPECT_FALSE(worker->isWorking());
}

TEST_F(UserCmdWorker_tests, processData_success) {
    UserCmdWorker userCmdWorker{ipcMemMock, ipcSocketMock, BUFFER_SIZE};
    worker = &userCmdWorker;
    WeatherData data;

    data.setTemperature(11.22);
    data.setPressure(1033.44);

    EXPECT_CALL(ipcMemMock, open()).Times(1);
    EXPECT_CALL(ipcSocketMock, open()).Times(1);
    EXPECT_CALL(ipcMemMock, read()).WillOnce(Return(data.serialize()));
    EXPECT_CALL(ipcSocketMock, read()).WillOnce(Invoke(
        []() {
            std::this_thread::sleep_for(25ms);
            return std::string{};
        }
    ));
    EXPECT_CALL(ipcSocketMock, close()).Times(1);

    worker->startWorking();
    EXPECT_TRUE(worker->isWorking());
    worker->processData();
}

TEST_F(UserCmdWorker_tests, stopWorking_when_exit_received) {
    UserCmdWorker userCmdWorker{ipcMemMock, ipcSocketMock, BUFFER_SIZE};
    worker = &userCmdWorker;

    EXPECT_CALL(ipcMemMock, open()).Times(1);
    EXPECT_CALL(ipcSocketMock, open()).Times(1);
    EXPECT_CALL(ipcMemMock, read()).WillOnce(Return(std::string{"exit"}));
    EXPECT_CALL(ipcSocketMock, read()).WillOnce(Invoke(
        []() {
            std::this_thread::sleep_for(25ms);
            return std::string{};
        }
    ));
    EXPECT_CALL(ipcSocketMock, close()).Times(1);
    EXPECT_CALL(ipcMemMock, close()).Times(1);

    worker->startWorking();
    EXPECT_TRUE(worker->isWorking());
    worker->processData();
}

} // anonymous