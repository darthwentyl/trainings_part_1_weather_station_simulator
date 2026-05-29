    #include <gtest/gtest.h>

#include <mocks/ipc/IpcMock.h>

#include <proc_managers/workers/handlers/UserCmdHelpHandler.h>

#include <sstream>

namespace {

using namespace testing;
using namespace mw::mocks;
using namespace mw::proc_managers::workers::handlers;

class UserCmdHelpHandler_tests : public Test {
public:
    ~UserCmdHelpHandler_tests() = default;

protected:
    StrictMock<IpcMock> ipcMock;
};

TEST_F(UserCmdHelpHandler_tests, show_help_by_user_request) {
    UserCmdHelpHandler handler{ipcMock};

    std::ostringstream oss;
    oss << "Available command: " << std::endl
        << "\ttemperature    \t- return last 10 temperature measurements" << std::endl
        << "\ttemperature <n>\t- return last n temperature measurements" << std::endl
        << "\texit           \t- disconnect from application - enter also disconnect" << std::endl
        << "\thelp           \t- show this help" << std::endl;

        EXPECT_CALL(ipcMock, write(StrEq(oss.str()))).WillOnce(Return(true));

        EXPECT_TRUE(handler.handle("help"));
}

TEST_F(UserCmdHelpHandler_tests, show_help_by_user_mistake) {
    UserCmdHelpHandler handler{ipcMock};

    const std::string command = "mistake";
    std::ostringstream oss;
    oss << "Wrong command: " << command << std::endl
        << "Available command: " << std::endl
        << "\ttemperature    \t- return last 10 temperature measurements" << std::endl
        << "\ttemperature <n>\t- return last n temperature measurements" << std::endl
        << "\texit           \t- disconnect from application - enter also disconnect" << std::endl
        << "\thelp           \t- show this help" << std::endl;

    EXPECT_CALL(ipcMock, write(StrEq(oss.str()))).WillOnce(Return(true));

    EXPECT_TRUE(handler.handle(command));
}

} // anonymous