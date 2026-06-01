#include <gtest/gtest.h>

#include <mocks/ipc/IpcMock.h>

#include <proc_managers/workers/handlers/UserCmdPressureHandler.h>

#include <format>

namespace {

using namespace testing;
using namespace mw::mocks;
using namespace mw::proc_managers::workers::handlers;
using namespace mw::proc_managers::workers;
using namespace mw::helpers;

using WeatherDatas = FixedDataContainer<WeatherData>;

constexpr const std::size_t ITEMS_NUM = 3;

class UserCmdPressureHandler_tests : public Test {
public:
    ~UserCmdPressureHandler_tests() = default;

protected:
    StrictMock<IpcMock> ipcMock;
};

TEST_F(UserCmdPressureHandler_tests, get_empty_data_measurements) {
    WeatherDatas datas{ITEMS_NUM};
    UserCmdPressureHandler handler{ipcMock, datas};

    const std::string cmd{"pressure"};
    const std::string response{"No pressure measurement, now\n"};

    EXPECT_CALL(ipcMock, write(StrEq(response))).WillOnce(Return(true));

    EXPECT_TRUE(handler.handle(cmd));
}

TEST_F(UserCmdPressureHandler_tests, get_more_data_than_in_container) {
    WeatherDatas datas{ITEMS_NUM};
    UserCmdPressureHandler handler{ipcMock, datas};

    WeatherData item{};
    item.setPressure(1000.11);
    datas.pushBack(item);

    const std::string cmd{"pressure 2"};
    std::ostringstream oss;
    oss << "Pressure:" << std::endl
        << std::format("{:3}. {:.>10.2f} [C]\n", 1, datas.getItem(0).getPressure());

    EXPECT_CALL(ipcMock, write(StrEq(oss.str()))).WillOnce(Return(true));

    EXPECT_TRUE(handler.handle(cmd));
}

TEST_F(UserCmdPressureHandler_tests, get_data_with_out_of_scope_elem) {
    WeatherDatas datas{ITEMS_NUM};
    UserCmdPressureHandler handler{ipcMock, datas};

    WeatherData item{};
    item.setPressure(989.11);
    datas.pushBack(item);
    item.setPressure(1000.2);
    datas.pushBack(item);
    item.setPressure(1000.33);
    datas.pushBack(item);

    const std::string cmd{"pressure 10"};
    std::ostringstream oss;
    oss << "Pressure:" << std::endl;
    for (std::size_t i = datas.getItems().size(); i > 0; --i) {
        oss << std::format("{:3}. {:.>10.2f} [C]\n", i, datas.getItem(i - 1).getPressure());
    }

    EXPECT_CALL(ipcMock, write(StrEq(oss.str()))).WillOnce(Return(true));

    EXPECT_TRUE(handler.handle(cmd));
}

TEST_F(UserCmdPressureHandler_tests, get_last_2_items_when_buff_is_full) {
    WeatherDatas datas{ITEMS_NUM};
    UserCmdPressureHandler handler{ipcMock, datas};

    WeatherData item{};
    item.setPressure(989.11);
    datas.pushBack(item);
    item.setPressure(1000.2);
    datas.pushBack(item);
    item.setPressure(1000.33);
    datas.pushBack(item);

    const std::string cmd{"pressure 2"};
    const std::size_t size = datas.getItems().size();

    std::ostringstream oss;
    oss << "Pressure:" << std::endl;
    for (std::size_t i = size; i > (size - 2); --i) {
        oss << std::format("{:3}. {:.>10.2f} [C]\n", i, datas.getItem(i - 1).getPressure());
    }

    EXPECT_CALL(ipcMock, write(StrEq(oss.str()))).WillOnce(Return(true));

    EXPECT_TRUE(handler.handle(cmd));
}

} // anonymous