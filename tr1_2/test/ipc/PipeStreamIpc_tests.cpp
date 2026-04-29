#include <gtest/gtest.h>

#include <mocks/StdLibStaticMock.h>

#include <ipc/PipeStreamIpc.h>
#include <exceptions/pipe_error.h>

namespace {

using namespace testing;
using namespace mw::mocks;
using namespace mw::ipc;
using namespace mw::exceptions;

constexpr const char* TEST_COMMAND = "test_command";
constexpr const char* READ_MODE = "r";
constexpr const char* WRITE_MODE = "w";

class PipeStreamIpc_tests : public Test {
public:
    ~PipeStreamIpc_tests() = default;
};

TEST_F(PipeStreamIpc_tests, open_success_read) {
    auto& stdLib = StdLibStaticMock::get();
    FILE file;

    EXPECT_CALL(stdLib, popen(StrEq(TEST_COMMAND), StrEq(READ_MODE))).WillOnce(Return(&file));

    try {
        auto instance = PipeStreamIpc{TEST_COMMAND, EPipeMode::READ};
        instance.open();
        EXPECT_TRUE(true);
    } catch (...) {
        EXPECT_FALSE(true);
    }
}

} // anonymous