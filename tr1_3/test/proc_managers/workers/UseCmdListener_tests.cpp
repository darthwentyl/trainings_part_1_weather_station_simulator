#include <gtest/gtest.h>

#include <proc_managers/workers/UserCmdListener.h>

#include <mocks/ipc/IpcMock.h>

#include <chrono>

namespace {

using namespace testing;
using namespace mw::mocks;
using namespace mw::proc_managers::workers;
using namespace std::chrono_literals;

constexpr const std::size_t BUFFER_SIZE = 2;
constexpr const char* UNKNOW_CMD = "unknow command";
constexpr const char* TEMPERATURE_CMD = "temperature";
constexpr const char* EXIT_CMD = "exit";

class UserCmdListener_tests : public Test {
public:
    ~UserCmdListener_tests() = default;

protected:
    StrictMock<IpcMock> ipcMock;
};

TEST_F(UserCmdListener_tests, start_stop_outside_listening) {
    UserCmdListener listener{ipcMock, BUFFER_SIZE};

    std::size_t countRead = 0;
    EXPECT_CALL(ipcMock, open()).Times(1);
    EXPECT_CALL(ipcMock, read()).Times(3).WillRepeatedly(
        Invoke([&]() -> std::string {
            std::this_thread::sleep_for(25ms);
            ++countRead;
            if (countRead < 3)
                return std::string{UNKNOW_CMD}; // I don't want to finish by handleCommand
            else
                return std::string{EXIT_CMD}; // Simulation exit by ClientTelnetHelper::cancelAccept()
        }
    ));
    EXPECT_CALL(ipcMock, close()).Times(1);

    listener.startListening();
    std::this_thread::sleep_for(50ms);
    listener.stopListening();
}

TEST_F(UserCmdListener_tests, start_2_times_stop_outside_listening) {
    UserCmdListener listener{ipcMock, BUFFER_SIZE};

    std::size_t countRead = 0;
    EXPECT_CALL(ipcMock, open()).Times(1);
    EXPECT_CALL(ipcMock, read()).Times(3).WillRepeatedly(
        Invoke([&]() -> std::string {
            std::this_thread::sleep_for(25ms);
            ++countRead;
            if (countRead < 3)
                return std::string{UNKNOW_CMD}; // I don't want to finish by handleCommand
            else
                return std::string{EXIT_CMD}; // Simulation exit by ClientTelnetHelper::cancelAccept()
        }
    ));
    EXPECT_CALL(ipcMock, close()).Times(1);

    listener.startListening();
    listener.startListening();
    std::this_thread::sleep_for(50ms);
    listener.stopListening();
}

TEST_F(UserCmdListener_tests, start_stop_2_times_outside_listening) {
    UserCmdListener listener{ipcMock, BUFFER_SIZE};

    std::size_t countRead = 0;
    EXPECT_CALL(ipcMock, open()).Times(1);
    EXPECT_CALL(ipcMock, read()).Times(3).WillRepeatedly(
        Invoke([&]() -> std::string {
            std::this_thread::sleep_for(25ms);
            ++countRead;
            if (countRead < 3)
                return std::string{UNKNOW_CMD}; // I don't want to finish by handleCommand
            else
                return std::string{EXIT_CMD};
        }
    ));
    EXPECT_CALL(ipcMock, close()).Times(1);

    listener.startListening();
    std::this_thread::sleep_for(50ms);
    listener.stopListening();
    listener.stopListening();
}

TEST_F(UserCmdListener_tests, start_stop_by_disconnect_client) {
    UserCmdListener listener{ipcMock, BUFFER_SIZE};

    EXPECT_CALL(ipcMock, open()).Times(1);
    EXPECT_CALL(ipcMock, read())
        .WillOnce(Return(std::string{UNKNOW_CMD}))
        .WillOnce(Return(std::string{}));
    EXPECT_CALL(ipcMock, close()).Times(1);

    listener.startListening();
}

TEST_F(UserCmdListener_tests, start_stop_by_exit_command) {
    UserCmdListener listener{ipcMock, BUFFER_SIZE};

    EXPECT_CALL(ipcMock, open()).Times(1);
    EXPECT_CALL(ipcMock, read())
        .WillOnce(Return(std::string{UNKNOW_CMD}))
        .WillOnce(Return(std::string{EXIT_CMD}));
    EXPECT_CALL(ipcMock, close()).Times(1);

    listener.startListening();
}

TEST_F(UserCmdListener_tests, temperature_all_measurements_command) {
    UserCmdListener listener{ipcMock, BUFFER_SIZE};

    EXPECT_CALL(ipcMock, open()).Times(1);
    EXPECT_CALL(ipcMock, read())
        .WillOnce(Return(std::string{TEMPERATURE_CMD}))
        .WillOnce(Return(std::string{}));
    EXPECT_CALL(ipcMock, close()).Times(1);

    WeatherData item;
    item.setTemperature(11.11);
    listener.addWeatherData(item);
    item.setTemperature(22.22);
    listener.addWeatherData(item);

    std::ostringstream oss;
    oss << "Temperature:" << std::endl
        << "\t" << 2 << ".\t" << 22.22 << "[C]" << std::endl
        << "\t" << 1 << ".\t" << 11.11 << "[C]" << std::endl;

    EXPECT_CALL(ipcMock, write(StrEq(oss.str()))).WillOnce(Return(true));

    listener.startListening();
}

TEST_F(UserCmdListener_tests, temperature_1_measurement_command) {
    UserCmdListener listener{ipcMock, BUFFER_SIZE};

    EXPECT_CALL(ipcMock, open()).Times(1);
    EXPECT_CALL(ipcMock, read())
        .WillOnce(Return(std::string{TEMPERATURE_CMD} + std::string{" 1"}))
        .WillOnce(Return(std::string{EXIT_CMD}));
    EXPECT_CALL(ipcMock, close()).Times(1);

    WeatherData item;
    item.setTemperature(11.11);
    listener.addWeatherData(item);
    item.setTemperature(22.22);
    listener.addWeatherData(item);

    std::ostringstream oss;
    oss << "Temperature:" << std::endl
        << "\t" << 2 << ".\t" << 22.22 << "[C]" << std::endl;

    EXPECT_CALL(ipcMock, write(StrEq(oss.str()))).WillOnce(Return(true));

    listener.startListening();
}

} // anonymous