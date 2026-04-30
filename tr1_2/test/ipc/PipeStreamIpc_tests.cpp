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
constexpr const int SUCCESS = 0;
constexpr const int FAILURE = -1;

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

TEST_F(PipeStreamIpc_tests, open_success_write) {
    auto& stdLib = StdLibStaticMock::get();
    FILE file;

    EXPECT_CALL(stdLib, popen(StrEq(TEST_COMMAND), StrEq(WRITE_MODE))).WillOnce(Return(&file));

    try {
        auto instance = PipeStreamIpc{TEST_COMMAND, EPipeMode::WRITE};
        instance.open();
        EXPECT_TRUE(true);
    } catch (...) {
        EXPECT_FALSE(true);
    }
}

TEST_F(PipeStreamIpc_tests, open_double_time) {
    auto& stdLib = StdLibStaticMock::get();
    FILE file;

    EXPECT_CALL(stdLib, popen(StrEq(TEST_COMMAND), StrEq(READ_MODE))).WillOnce(Return(&file));

    try {
        auto instance = PipeStreamIpc{TEST_COMMAND, EPipeMode::READ};
        instance.open();
        instance.open();
        EXPECT_TRUE(true);
    } catch (...) {
        EXPECT_FALSE(true);
    }
}

TEST_F(PipeStreamIpc_tests, open_command_empty) {
    try {
        auto instance = PipeStreamIpc{std::string{}, EPipeMode::WRITE};
        instance.open();
        EXPECT_FALSE(true);
    } catch (const pipe_error& e) {
        std::cout << __PRETTY_FUNCTION__ << ":" << __LINE__ << e.what() << std::endl;
        EXPECT_TRUE(true);
    } catch (...) {
        EXPECT_FALSE(true);
    }
}

TEST_F(PipeStreamIpc_tests, open_popen_failed) {
    auto& stdLib = StdLibStaticMock::get();

    EXPECT_CALL(stdLib, popen(_, _)).WillOnce(Return(nullptr));

    try {
        auto instance = PipeStreamIpc{TEST_COMMAND, EPipeMode::READ};
        instance.open();
        EXPECT_FALSE(true);
    } catch (const pipe_error& e) {
        std::cout << __PRETTY_FUNCTION__ << ":" << __LINE__ << e.what() << std::endl;
        EXPECT_TRUE(true);
    } catch (...) {
        EXPECT_FALSE(true);
    }
}

TEST_F(PipeStreamIpc_tests, close_success) {
    auto& stdLib = StdLibStaticMock::get();
    FILE file;

    EXPECT_CALL(stdLib, popen(_, _)).WillOnce(Return(&file));
    EXPECT_CALL(stdLib, pclose(&file)).WillOnce(Return(SUCCESS));

    try {
        auto instance = PipeStreamIpc{TEST_COMMAND, EPipeMode::READ};
        instance.open();
        instance.close();
        EXPECT_TRUE(true);
    } catch (...) {
        EXPECT_FALSE(true);
    }
}

TEST_F(PipeStreamIpc_tests, close_stream_nullptr) {
    auto& stdLib = StdLibStaticMock::get();

    EXPECT_CALL(stdLib, pclose(_)).Times(0);

    try {
        auto instance = PipeStreamIpc{TEST_COMMAND, EPipeMode::WRITE};
        instance.close();
        EXPECT_TRUE(true);
    } catch (...) {
        EXPECT_FALSE(true);
    }
}

TEST_F(PipeStreamIpc_tests, close_pclose_failure) {
    auto& stdLib = StdLibStaticMock::get();
    FILE file;

    EXPECT_CALL(stdLib, popen(_, _)).WillOnce(Return(&file));
    EXPECT_CALL(stdLib, pclose(&file)).WillOnce(Return(FAILURE));

    try {
        auto instance = PipeStreamIpc{TEST_COMMAND, EPipeMode::READ};
        instance.open();
        instance.close();
        EXPECT_FALSE(true);
    } catch (const pipe_error& e) {
        std::cout << __PRETTY_FUNCTION__ << ":" << __LINE__ << ": " << e.what() << std::endl;
        EXPECT_TRUE(true);
    } catch (...) {
        EXPECT_FALSE(true);
    }
}

} // anonymous