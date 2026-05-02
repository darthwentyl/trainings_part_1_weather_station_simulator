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
    EXPECT_CALL(stdLib, pclose(_)).WillOnce(Return(SUCCESS));

    try {
        auto instance = PipeStreamIpc{TEST_COMMAND, EPipeMode::READ};
        instance.open();
        EXPECT_TRUE(true);
    } catch (const std::exception& e) {
        std::cout << __PRETTY_FUNCTION__ << ":" << __LINE__ << ": " << e.what() << std::endl;
        EXPECT_FALSE(true);
    }
}

TEST_F(PipeStreamIpc_tests, open_success_write) {
    auto& stdLib = StdLibStaticMock::get();
    FILE file;

    EXPECT_CALL(stdLib, popen(StrEq(TEST_COMMAND), StrEq(WRITE_MODE))).WillOnce(Return(&file));
    EXPECT_CALL(stdLib, pclose(_)).WillOnce(Return(SUCCESS));

    try {
        auto instance = PipeStreamIpc{TEST_COMMAND, EPipeMode::WRITE};
        instance.open();
        EXPECT_TRUE(true);
    } catch (const std::exception& e) {
        std::cout << __PRETTY_FUNCTION__ << ":" << __LINE__ << ": " << e.what() << std::endl;
        EXPECT_FALSE(true);
    }
}

TEST_F(PipeStreamIpc_tests, open_double_time) {
    auto& stdLib = StdLibStaticMock::get();
    FILE file;

    EXPECT_CALL(stdLib, popen(StrEq(TEST_COMMAND), StrEq(READ_MODE))).WillOnce(Return(&file));
    EXPECT_CALL(stdLib, pclose(_)).WillOnce(Return(SUCCESS));

    try {
        auto instance = PipeStreamIpc{TEST_COMMAND, EPipeMode::READ};
        instance.open();
        instance.open();
        EXPECT_TRUE(true);
    } catch (const std::exception& e) {
        std::cout << __PRETTY_FUNCTION__ << ":" << __LINE__ << ": " << e.what() << std::endl;
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
    } catch (const std::exception& e) {
        std::cout << __PRETTY_FUNCTION__ << ":" << __LINE__ << ": " << e.what() << std::endl;
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
    } catch (const std::exception& e) {
        std::cout << __PRETTY_FUNCTION__ << ":" << __LINE__ << ": " << e.what() << std::endl;
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
    } catch (const std::exception& e) {
        std::cout << __PRETTY_FUNCTION__ << ":" << __LINE__ << ": " << e.what() << std::endl;
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
    } catch (const std::exception& e) {
        std::cout << __PRETTY_FUNCTION__ << ":" << __LINE__ << ": " << e.what() << std::endl;
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
    } catch (const std::exception& e) {
        std::cout << __PRETTY_FUNCTION__ << ":" << __LINE__ << ": " << e.what() << std::endl;
        EXPECT_FALSE(true);
    }
}

TEST_F(PipeStreamIpc_tests, close_in_destructor_pclose_failure) {
    auto& stdLib = StdLibStaticMock::get();
    FILE file;

    EXPECT_CALL(stdLib, popen(_, _)).WillOnce(Return(&file));
    EXPECT_CALL(stdLib, pclose(&file)).WillOnce(Return(FAILURE));
    try {
        auto instance = PipeStreamIpc{TEST_COMMAND, EPipeMode::WRITE};
        instance.open();
        EXPECT_TRUE(true);
    } catch (const std::exception& e) {
        std::cout << __PRETTY_FUNCTION__ << ":" << __LINE__ << ": " << e.what() << std::endl;
        EXPECT_FALSE(true);
    }
}

TEST_F(PipeStreamIpc_tests, read_success) {
    auto& stdLib = StdLibStaticMock::get();
    FILE file;
    const std::string buff = "abcd";

    EXPECT_CALL(stdLib, popen(_, _)).WillOnce(Return(&file));
    EXPECT_CALL(stdLib, fgets(_, _, _))
    .WillOnce(
        Invoke([=](char* s, int, FILE*) {
            strncpy(s, buff.c_str(), buff.size());
            s[buff.size()] = '\0';
            return s;
        }
    ))
    .WillOnce(Return(nullptr));
    EXPECT_CALL(stdLib, pclose(_)).WillOnce(Return(SUCCESS));

    try {
        auto instance = PipeStreamIpc{TEST_COMMAND, EPipeMode::READ};
        instance.open();
        EXPECT_STREQ(buff.c_str(), instance.read().c_str());
    } catch (const std::exception& e) {
        std::cout << __PRETTY_FUNCTION__ << ":" << __LINE__ << ": " << e.what() << std::endl;
        EXPECT_FALSE(true);
    }
}

TEST_F(PipeStreamIpc_tests, read_success_buffer_read_2_times) {
    auto& stdLib = StdLibStaticMock::get();
    FILE file;
    const std::string buff = "abcd";

    auto fgetsImpl = [buff](char* s, int, FILE*) -> char* {
        strncpy(s, buff.c_str(), buff.size());
        s[buff.size()] = '\0';
        return s;
    };

    EXPECT_CALL(stdLib, popen(_, _)).WillOnce(Return(&file));
    EXPECT_CALL(stdLib, fgets(_, _, _))
    .WillOnce(Invoke(fgetsImpl))
    .WillOnce(Invoke(fgetsImpl))
    .WillOnce(Return(nullptr));
    EXPECT_CALL(stdLib, pclose(_)).WillOnce(Return(SUCCESS));

    try {
        auto instance = PipeStreamIpc{TEST_COMMAND, EPipeMode::READ};
        instance.open();
        const std::string comparison = buff + buff;
        EXPECT_STREQ(comparison.c_str(), instance.read().c_str());
    } catch (const std::exception& e) {
        std::cout << __PRETTY_FUNCTION__ << ":" << __LINE__ << ": " << e.what() << std::endl;
        EXPECT_FALSE(true);
    }
}

TEST_F(PipeStreamIpc_tests, read_nullptr) {
    auto& stdLib = StdLibStaticMock::get();
    FILE file;
    EXPECT_CALL(stdLib, popen(_, _)).WillOnce(Return(&file));
    EXPECT_CALL(stdLib, fgets(_, _, _)).WillOnce(Return(nullptr));
    EXPECT_CALL(stdLib, pclose(_)).WillOnce(Return(SUCCESS));

    try {
        auto instance = PipeStreamIpc{TEST_COMMAND, EPipeMode::READ};
        instance.open();
        EXPECT_STREQ(std::string{}.c_str(), instance.read().c_str());
    } catch (const std::exception& e) {
        std::cout << __PRETTY_FUNCTION__ << ":" << __LINE__ << ": " << e.what() << std::endl;
        EXPECT_FALSE(true);
    }
}

TEST_F(PipeStreamIpc_tests, read_not_started_yet) {
    try {
        auto instance = PipeStreamIpc{TEST_COMMAND, EPipeMode::READ};
        instance.read();
        EXPECT_FALSE(true);
    } catch (const pipe_error& e) {
        std::cout << __PRETTY_FUNCTION__ << ":" << __LINE__ << ": " << e.what() << std::endl;
        EXPECT_TRUE(true);
    } catch (const std::exception& e) {
        std::cout << __PRETTY_FUNCTION__ << ":" << __LINE__ << ": " << e.what() << std::endl;
        EXPECT_FALSE(true);
    }
}

TEST_F(PipeStreamIpc_tests, read_but_it_write_mode) {
    auto& stdLib = StdLibStaticMock::get();
    FILE file;
    EXPECT_CALL(stdLib, popen(_, _)).WillOnce(Return(&file));
    EXPECT_CALL(stdLib, pclose(_)).WillOnce(Return(SUCCESS));

    try {
        auto instance = PipeStreamIpc{TEST_COMMAND, EPipeMode::WRITE};
        instance.open();
        instance.read();
        EXPECT_FALSE(true);
    } catch (const pipe_error& e) {
        std::cout << __PRETTY_FUNCTION__ << ":" << __LINE__ << ": " << e.what() << std::endl;
        EXPECT_TRUE(true);
    } catch (const std::exception& e) {
        std::cout << __PRETTY_FUNCTION__ << ":" << __LINE__ << ": " << e.what() << std::endl;
        EXPECT_FALSE(true);
    }
}

TEST_F(PipeStreamIpc_tests, writer_success) {
    auto& stdLib = StdLibStaticMock::get();
    FILE file;
    const std::string& msg = "abcd";
    EXPECT_CALL(stdLib, popen(_, _)).WillOnce(Return(&file));
    EXPECT_CALL(stdLib, fputs(StrEq(msg), &file)).WillOnce(Return(msg.size()));
    EXPECT_CALL(stdLib, pclose(_)).WillOnce(Return(SUCCESS));

    try{
        auto instance = PipeStreamIpc{TEST_COMMAND, EPipeMode::WRITE};
        instance.open();
        EXPECT_TRUE(instance.write(msg));
    } catch  (const std::exception& e) {
        std::cout << __PRETTY_FUNCTION__ << ":" << __LINE__ << ": " << e.what() << std::endl;
        EXPECT_FALSE(true);
    }
}

TEST_F(PipeStreamIpc_tests, write_stream_nullptr) {
    const std::string& msg = "abcd";

    try {
        auto instance = PipeStreamIpc{TEST_COMMAND, EPipeMode::WRITE};
        EXPECT_FALSE(instance.write(msg));
    } catch (const pipe_error& e) {
        std::cout << __PRETTY_FUNCTION__ << ":" << __LINE__ << ": " << e.what() << std::endl;
        EXPECT_TRUE(true);
    } catch (const std::exception& e) {
        std::cout << __PRETTY_FUNCTION__ << ":" <<  __LINE__ << ": " << e.what() << std::endl;
        EXPECT_FALSE(false);
    }
}

TEST_F(PipeStreamIpc_tests, write_when_you_are_in_read_mode) {
    auto& stdLib = StdLibStaticMock::get();
    FILE file;
    const std::string& msg = "abcd";

    EXPECT_CALL(stdLib, popen(_, _)).WillOnce(Return(&file));
    EXPECT_CALL(stdLib, pclose(_)).WillOnce(Return(SUCCESS));

    try {
        auto instance = PipeStreamIpc{TEST_COMMAND, EPipeMode::READ};
        instance.open();
        EXPECT_FALSE(instance.write(msg));
    } catch (const pipe_error& e) {
        std::cout << __PRETTY_FUNCTION__ << ":" << __LINE__ << ": " << e.what() << std::endl;
        EXPECT_TRUE(true);
    } catch (const std::exception& e) {
        std::cout << __PRETTY_FUNCTION__ << ": " << __LINE__ <<  ": " << e.what() << std::endl;
        EXPECT_FALSE(true);
    }
}

TEST_F(PipeStreamIpc_tests, write_fputs_EOF) {
    auto& stdLib = StdLibStaticMock::get();
    FILE file;
    const std::string& msg = "abcd";

    EXPECT_CALL(stdLib, popen(_, _)).WillOnce(Return(&file));
    EXPECT_CALL(stdLib, fputs(StrEq(msg), &file)).WillOnce(Return(EOF));
    EXPECT_CALL(stdLib, pclose(_)).WillOnce(Return(SUCCESS));

    try {
        auto instance = PipeStreamIpc{TEST_COMMAND, EPipeMode::WRITE};
        instance.open();
        EXPECT_FALSE(instance.write(msg));
    } catch (const pipe_error& e) {
        std::cout << __PRETTY_FUNCTION__ << ":" << __LINE__ << ": " << e.what() << std::endl;
        EXPECT_TRUE(true);
    } catch (const std::exception& e) {
        std::cout << __PRETTY_FUNCTION__ << ":" << __LINE__ << ": " << e.what() << std::endl;
        EXPECT_FALSE(false);
    }
}

} // anonymous