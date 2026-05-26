#include <gtest/gtest.h>

#include <mocks/ipc/IpcMock.h>

#include <proc_managers/workers/handlers/UserCmdTemperatureHandler.h>

namespace {

using namespace testing;
using namespace mw::mocks;
using namespace mw::proc_managers::workers::handlers;
using namespace mw::proc_managers::workers;
using namespace mw::helpers;

using WeatherDatas = FixedDataContainer<WeatherData>;

constexpr const std::size_t ITEMS_NUM = 3;

class UserCmdTemperatureHandler_tests : public Test {
public:
    ~UserCmdTemperatureHandler_tests() = default;

protected:
    StrictMock<IpcMock> ipcMock;
};

TEST_F(UserCmdTemperatureHandler_tests, get_empty_data_measurements) {
    WeatherDatas datas{ITEMS_NUM};
    UserCmdTemperatureHandler handler{ipcMock, datas};

    const std::string cmd{"temperature"};
    const std::string response{"No temperature measurement, now\n"};

    EXPECT_CALL(ipcMock, write(StrEq(response))).WillOnce(Return(true));

    EXPECT_TRUE(handler.handle(cmd));
}

TEST_F(UserCmdTemperatureHandler_tests, get_more_data_than_in_container) {
    WeatherDatas datas{ITEMS_NUM};
    UserCmdTemperatureHandler handler{ipcMock, datas};

    WeatherData item{};
    item.setTemperature(29.12);
    datas.pushBack(item);

    const std::string cmd{"temperature 2"};
    std::ostringstream oss;
    oss << "Temperature:" << std::endl
        << "\t" << 1 << ".\t"
        << datas.getItem(0).getTemperature()
        << "[C]" << std::endl;

    EXPECT_CALL(ipcMock, write(StrEq(oss.str()))).WillOnce(Return(true));

    EXPECT_TRUE(handler.handle(cmd));
}

TEST_F(UserCmdTemperatureHandler_tests, get_data_with_out_of_scope_elem) {
    WeatherDatas datas{ITEMS_NUM};
    UserCmdTemperatureHandler handler{ipcMock, datas};

    WeatherData item{};
    item.setTemperature(1.11);
    datas.pushBack(item);
    item.setTemperature(2.22);
    datas.pushBack(item);
    item.setTemperature(3.33);
    datas.pushBack(item);

    const std::string cmd{"temperature 10"};
    std::ostringstream oss;
    oss << "Temperature:" << std::endl;
    for (std::size_t i = datas.getItems().size(); i > 0; --i) {
        oss << "\t" << i << ".\t" << datas.getItem(i - 1).getTemperature() << "[C]" << std::endl;
    }

    EXPECT_CALL(ipcMock, write(StrEq(oss.str()))).WillOnce(Return(true));

    EXPECT_TRUE(handler.handle(cmd));
}

TEST_F(UserCmdTemperatureHandler_tests, get_last_2_items_when_buff_is_full) {
    WeatherDatas datas{ITEMS_NUM};
    UserCmdTemperatureHandler handler{ipcMock, datas};

    WeatherData item{};
    item.setTemperature(1.11);
    datas.pushBack(item);
    item.setTemperature(2.22);
    datas.pushBack(item);
    item.setTemperature(3.33);
    datas.pushBack(item);

    const std::string cmd{"temperature 2"};
    const std::size_t size = datas.getItems().size();

    std::ostringstream oss;
    oss << "Temperature:" << std::endl;
    for (std::size_t i = size; i > (size - 2); --i) {
        oss << "\t" << i << ".\t" << datas.getItem(i - 1).getTemperature() << "[C]" << std::endl;
    }

    EXPECT_CALL(ipcMock, write(StrEq(oss.str()))).WillOnce(Return(true));

    EXPECT_TRUE(handler.handle(cmd));
}

} // anonymous