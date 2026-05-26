#include <gtest/gtest.h>

#include <proc_managers/workers/handlers/UserCmdExitHandler.h>

namespace {

using namespace testing;
using namespace mw::proc_managers::workers::handlers;

class UserCmdExitHandler_tests : public Test {
public:
    ~UserCmdExitHandler_tests() = default;

protected:
    UserCmdExitHandler exitHandler;
};

TEST_F(UserCmdExitHandler_tests, handle_exit) {
    EXPECT_FALSE(exitHandler.handle("exit"));
}

TEST_F(UserCmdExitHandler_tests, handle_empty_command) {
    EXPECT_FALSE(exitHandler.handle(std::string{}));
}

TEST_F(UserCmdExitHandler_tests, handle_incorrect_command) {
    EXPECT_TRUE(exitHandler.handle("incorrect command"));
}

} // anonymous