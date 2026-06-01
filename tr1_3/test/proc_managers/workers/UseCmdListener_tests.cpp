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
constexpr const char* PRESSURE_CMD = "pressure";
constexpr const char* HELP_CMD = "help";
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
    EXPECT_CALL(ipcMock, write(_)).Times(2).WillRepeatedly(Return(true));
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
    EXPECT_CALL(ipcMock, write(_)).Times(2).WillRepeatedly(Return(true));
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
    EXPECT_CALL(ipcMock, write(_)).Times(2).WillRepeatedly(Return(true));
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
        .WillRepeatedly(Return(std::string{}));
    EXPECT_CALL(ipcMock, write(_)).WillOnce(Return(true));
    EXPECT_CALL(ipcMock, close()).Times(1);


    listener.startListening();
}

TEST_F(UserCmdListener_tests, start_stop_by_exit_command) {
    UserCmdListener listener{ipcMock, BUFFER_SIZE};

    EXPECT_CALL(ipcMock, open()).Times(1);
    EXPECT_CALL(ipcMock, read())
        .WillOnce(Return(std::string{UNKNOW_CMD}))
        .WillRepeatedly(Return(std::string{EXIT_CMD}));
    EXPECT_CALL(ipcMock, write(_)).WillOnce(Return(true));
    EXPECT_CALL(ipcMock, close()).Times(1);

    listener.startListening();
}

TEST_F(UserCmdListener_tests, temperature_all_measurements_command) {
    UserCmdListener listener{ipcMock, BUFFER_SIZE};

    EXPECT_CALL(ipcMock, open()).Times(1);
    EXPECT_CALL(ipcMock, read())
        .WillOnce(Return(std::string{TEMPERATURE_CMD}))
        .WillRepeatedly(Return(std::string{}));
    EXPECT_CALL(ipcMock, close()).Times(1);

    WeatherData item;
    item.setTemperature(11.11);
    listener.addWeatherData(item);
    item.setTemperature(22.22);
    listener.addWeatherData(item);

    std::ostringstream oss;
    oss << "Temperature:" << std::endl
        << std::format("{:3}. {:.>10.2f} [C]\n", 2, 22.22)
        << std::format("{:3}. {:.>10.2f} [C]\n", 1, 11.11);

    EXPECT_CALL(ipcMock, write(StrEq(oss.str()))).WillOnce(Return(true));

    listener.startListening();
}

TEST_F(UserCmdListener_tests, temperature_1_measurement_command) {
    UserCmdListener listener{ipcMock, BUFFER_SIZE};

    EXPECT_CALL(ipcMock, open()).Times(1);
    EXPECT_CALL(ipcMock, read())
        .WillOnce(Return(std::string{TEMPERATURE_CMD} + std::string{" 1"}))
        .WillRepeatedly(Return(std::string{EXIT_CMD}));
    EXPECT_CALL(ipcMock, close()).Times(1);

    WeatherData item;
    item.setTemperature(11.11);
    listener.addWeatherData(item);
    item.setTemperature(22.22);
    listener.addWeatherData(item);

    std::ostringstream oss;
    oss << "Temperature:" << std::endl
        << std::format("{:3}. {:.>10.2f} [C]\n", 2, 22.22);

    EXPECT_CALL(ipcMock, write(StrEq(oss.str()))).WillOnce(Return(true));

    listener.startListening();
}

TEST_F(UserCmdListener_tests, pressure_all_measurements_command) {
    UserCmdListener listener{ipcMock, BUFFER_SIZE};

    EXPECT_CALL(ipcMock, open()).Times(1);
    EXPECT_CALL(ipcMock, read())
        .WillOnce(Return(std::string{PRESSURE_CMD}))
        .WillRepeatedly(Return(std::string{EXIT_CMD}));
    EXPECT_CALL(ipcMock, close()).Times(1);

    WeatherData item;
    item.setPressure(1000.11);
    listener.addWeatherData(item);
    item.setPressure(1000.22);
    listener.addWeatherData(item);

    std::ostringstream oss;
    oss << "Pressure:" << std::endl
        << std::format("{:3}. {:.>10.2f} [hPa]\n", 2, 1000.22)
        << std::format("{:3}. {:.>10.2f} [hPa]\n", 1, 1000.11);

    EXPECT_CALL(ipcMock, write(StrEq(oss.str()))).WillOnce(Return(true));

    listener.startListening();
}

TEST_F(UserCmdListener_tests, pressure_1_measurement_command) {
    UserCmdListener listener{ipcMock, BUFFER_SIZE};

    EXPECT_CALL(ipcMock, open()).Times(1);
    EXPECT_CALL(ipcMock, read())
        .WillOnce(Return(std::string{PRESSURE_CMD} + std::string{" 1"}))
        .WillRepeatedly(Return(std::string{}));
    EXPECT_CALL(ipcMock, close()).Times(1);

    WeatherData item;
    item.setPressure(1000.11);
    listener.addWeatherData(item);
    item.setPressure(1000.22);
    listener.addWeatherData(item);

    std::ostringstream oss;
    oss << "Pressure:" << std::endl
        << std::format("{:3}. {:.>10.2f} [hPa]\n", 2, 1000.22);

    EXPECT_CALL(ipcMock, write(StrEq(oss.str()))).WillOnce(Return(true));

    listener.startListening();
}

TEST_F(UserCmdListener_tests, help_command) {
    UserCmdListener listener{ipcMock, BUFFER_SIZE};

    EXPECT_CALL(ipcMock, open()).Times(1);
    EXPECT_CALL(ipcMock, read())
        .WillOnce(Return(std::string{HELP_CMD}))
        .WillRepeatedly(Return(std::string{EXIT_CMD}));
    EXPECT_CALL(ipcMock, close()).Times(1);

    std::ostringstream oss;
    oss << "Available command: " << std::endl
        << "\ttemperature    \t- return last 10 temperature measurements" << std::endl
        << "\ttemperature <n>\t- return last n temperature measurements" << std::endl
        << "\tpressure       \t- return last 10 pressure measurements" << std::endl
        << "\tpressure    <n>\t- return last n pressure measurements" << std::endl
        << "\texit           \t- disconnect from application - enter also disconnect" << std::endl
        << "\thelp           \t- show this help" << std::endl;

    EXPECT_CALL(ipcMock, write(StrEq(oss.str()))).WillOnce(Return(true));

    listener.startListening();
}

TEST_F(UserCmdListener_tests, help_by_unknown_command) {
    UserCmdListener listener{ipcMock, BUFFER_SIZE};

    EXPECT_CALL(ipcMock, open()).Times(1);
    EXPECT_CALL(ipcMock, read())
        .WillOnce(Return(std::string{UNKNOW_CMD}))
        .WillRepeatedly(Return(std::string{}));
    EXPECT_CALL(ipcMock, close()).Times(1);

    std::ostringstream oss;
    oss << "Wrong command: " << UNKNOW_CMD << std::endl
        <<"Available command: " << std::endl
        << "\ttemperature    \t- return last 10 temperature measurements" << std::endl
        << "\ttemperature <n>\t- return last n temperature measurements" << std::endl
        << "\tpressure       \t- return last 10 pressure measurements" << std::endl
        << "\tpressure    <n>\t- return last n pressure measurements" << std::endl
        << "\texit           \t- disconnect from application - enter also disconnect" << std::endl
        << "\thelp           \t- show this help" << std::endl;

    EXPECT_CALL(ipcMock, write(StrEq(oss.str()))).WillOnce(Return(true));

    listener.startListening();
}

} // anonymous