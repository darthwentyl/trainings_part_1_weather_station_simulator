#include <gtest/gtest.h>

#include <mocks/StdLibStaticMock.h>

#include <helpers/ConnectionSocketHelper.h>
#include <exceptions/socket_error.h>

namespace {

using namespace testing;
using namespace mw::mocks;
using namespace mw::helpers;
using namespace mw::exceptions;

constexpr const int FAILURE = -1;
constexpr const int SUCCESS = 0;

class ConnectionSocketHelper_tests : public Test {
public:
    ~ConnectionSocketHelper_tests() = default;
};

TEST_F(ConnectionSocketHelper_tests, acceptConnection_success) {
    auto& stdLib = StdLibStaticMock::get();
    ConnectionSocketHelper conn{};
    const int listenFd = 123;
    const int connectFd = 321;

    EXPECT_CALL(stdLib, accept(Eq(listenFd), Eq(nullptr), Eq(nullptr))).WillOnce(Return(connectFd));
    EXPECT_CALL(stdLib, close(_)).WillOnce(Return(SUCCESS));

    try {
        conn.acceptConnection(listenFd);
        EXPECT_TRUE(conn.isConnected());
    } catch (const std::exception& e) {
        std::cout << __PRETTY_FUNCTION__ << ":" << __LINE__ << ": " << e.what() << std::endl;
        EXPECT_FALSE(true);
    }
}

TEST_F(ConnectionSocketHelper_tests, acceptConnection_failed) {
    auto& stdLib = StdLibStaticMock::get();
    ConnectionSocketHelper conn{};
    const int listenFd = 123;

    EXPECT_CALL(stdLib, accept(Eq(listenFd), Eq(nullptr), Eq(nullptr))).WillOnce(Return(FAILURE));


    try {
        conn.acceptConnection(listenFd);
        EXPECT_FALSE(true);
    } catch (const socket_error& e) {
        std::cout << __PRETTY_FUNCTION__ << ":" << __LINE__ << ": " << e.what() << std::endl;
        EXPECT_FALSE(conn.isConnected());
    } catch (const std::exception& e) {
        std::cout << __PRETTY_FUNCTION__ << ":" << __LINE__ << ": " << e.what() << std::endl;
        EXPECT_FALSE(true);
    }
}

TEST_F(ConnectionSocketHelper_tests, closeSocket_destructor_success) {
    auto& stdLib = StdLibStaticMock::get();
    ConnectionSocketHelper conn{};
    const int listenFd = 123;
    const int connectFd = 321;

    EXPECT_CALL(stdLib, accept(_, _, _)).WillOnce(Return(connectFd));
    EXPECT_CALL(stdLib, close(Eq(connectFd))).WillOnce(Return(SUCCESS));

    try {
        conn.acceptConnection(listenFd);
        EXPECT_TRUE(conn.isConnected());
    } catch (const std::exception& e) {
        std::cout << __PRETTY_FUNCTION__ << ":" << __LINE__ << ": " << e.what() << std::endl;
        EXPECT_FALSE(true);
    }
}

TEST_F(ConnectionSocketHelper_tests, closeSocket_destructor_failure) {
    auto& stdLib = StdLibStaticMock::get();
    const int listenFd = 123;
    const int connectFd = 321;

    EXPECT_CALL(stdLib, accept(_, _, _)).WillOnce(Return(connectFd));
    EXPECT_CALL(stdLib, close(Eq(connectFd))).WillOnce(Return(FAILURE));
    try {
        ConnectionSocketHelper conn{};
        conn.acceptConnection(listenFd);
        EXPECT_TRUE(conn.isConnected());
    } catch (const std::exception& e) {
        std::cout << __PRETTY_FUNCTION__ << ":" << __LINE__ << ": " << e.what() << std::endl;
        EXPECT_FALSE(true);
    }
}

} // anonymous