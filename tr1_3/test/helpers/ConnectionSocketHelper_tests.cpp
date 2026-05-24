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
constexpr const int BUFF_SIZE = 128;

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

TEST_F(ConnectionSocketHelper_tests, closeSocket_success) {
    auto& stdLib = StdLibStaticMock::get();
    ConnectionSocketHelper conn{};
    const int listenFd = 123;
    const int connectFd = 321;

    EXPECT_CALL(stdLib, accept(_, _, _)).WillOnce(Return(connectFd));
    EXPECT_CALL(stdLib, close(Eq(connectFd))).WillOnce(Return(SUCCESS));
    try {
        conn.acceptConnection(listenFd);
        EXPECT_TRUE(conn.isConnected());
        conn.closeConnection();
        EXPECT_FALSE(conn.isConnected());
    } catch (const std::exception& e) {
        std::cout << __PRETTY_FUNCTION__ << ":" << __LINE__ << ": " << e.what() << std::endl;
        EXPECT_FALSE(true);
    }
}

TEST_F(ConnectionSocketHelper_tests, closeSocket_failure) {
    auto& stdLib = StdLibStaticMock::get();
    ConnectionSocketHelper conn{};
    const int listenFd = 123;
    const int connectFd = 321;

    EXPECT_CALL(stdLib, accept(_, _, _)).WillOnce(Return(connectFd));
    EXPECT_CALL(stdLib, close(Eq(connectFd))).WillOnce(Return(FAILURE));
    try {
        conn.acceptConnection(listenFd);
        conn.closeConnection();
        EXPECT_FALSE(true);
    } catch (const socket_error& e) {
        std::cout << __PRETTY_FUNCTION__ << ":" << __LINE__ << ": " << e.what() << std::endl;
        EXPECT_TRUE(true);
    } catch (const std::exception& e) {
        std::cout << __PRETTY_FUNCTION__ << ":" << __LINE__ << ": " << e.what() << std::endl;
    }
}

TEST_F(ConnectionSocketHelper_tests, readData_success) {
    auto& stdLib = StdLibStaticMock::get();
    ConnectionSocketHelper conn{};
    const int listenFd = 123;
    const int connectFd = 321;

    const std::string msg = "message\n\r";

    EXPECT_CALL(stdLib, accept(_, _, _)).WillOnce(Return(connectFd));
    EXPECT_CALL(stdLib, read(Eq(connectFd), _, _))
    .WillOnce(
        Invoke([=](int, void* buf, size_t) -> ssize_t {
            strncpy(static_cast<char*>(buf), msg.c_str(), msg.size());
            return msg.size();
        }
    ));
    EXPECT_CALL(stdLib, close(_)).WillOnce(Return(SUCCESS));

    try {
        conn.acceptConnection(listenFd);
        EXPECT_STREQ(conn.readData().c_str(), "message");
    } catch (const std::exception& e) {
        std::cout << __PRETTY_FUNCTION__ << ":" << __LINE__ << ": " << e.what() << std::endl;
        EXPECT_FALSE(true);
    }
}

TEST_F(ConnectionSocketHelper_tests, readData_big_buff) {
    auto& stdLib = StdLibStaticMock::get();
    ConnectionSocketHelper conn{};
    const int listenFd = 123;
    const int connectFd = 321;

    const std::string msg(BUFF_SIZE, 'a');
    const std::string empty = "\n\r";

    auto read_msg = [msg](int, void* buf, size_t) -> ssize_t {
        strncpy(static_cast<char*>(buf), msg.c_str(), msg.size());
        return msg.size();
    };

    EXPECT_CALL(stdLib, accept(_, _, _)).WillOnce(Return(connectFd));
    EXPECT_CALL(stdLib, read(Eq(connectFd), _, _))
    .WillOnce(Invoke(read_msg))
    .WillOnce(Invoke(read_msg))
    .WillOnce(
        Invoke([=](int, void* buf, size_t) -> ssize_t {
            strncpy(static_cast<char*>(buf), empty.c_str(), empty.size());
            return empty.size();
        }
    ));
    EXPECT_CALL(stdLib, close(_)).WillOnce(Return(SUCCESS));

    try {
        conn.acceptConnection(listenFd);
        std::string compMsg = msg + msg;
        EXPECT_STREQ(conn.readData().c_str(), compMsg.c_str());
    } catch (const std::exception& e) {
        std::cout << __PRETTY_FUNCTION__ << ":" << __LINE__ << ": " << e.what() << std::endl;
        EXPECT_FALSE(true);
    }
}

TEST_F(ConnectionSocketHelper_tests, readData_not_connected) {
    ConnectionSocketHelper conn{};
    try {
        conn.readData();
        EXPECT_FALSE(true);
    } catch (const socket_error& e) {
        std::cout << __PRETTY_FUNCTION__ << ":" << __LINE__ << ": " << e.what() << std::endl;
        EXPECT_TRUE(true);
    } catch (const std::exception& e) {
        std::cout << __PRETTY_FUNCTION__ << ":" << __LINE__ << ": " << e.what() << std::endl;
        EXPECT_FALSE(true);
    }
}

TEST_F(ConnectionSocketHelper_tests, writeData_success) {
    auto& stdLib = StdLibStaticMock::get();
    ConnectionSocketHelper conn{};
    const int listenFd = 123;
    const int connectFd = 321;

    const std::string msg = "abcd";
    std::string receivedData;

    EXPECT_CALL(stdLib, accept(_, _, _)).WillOnce(Return(connectFd));
    EXPECT_CALL(stdLib, write(Eq(connectFd), _, _))
    .WillOnce(
        Invoke([&](int, const void* buf, size_t) -> ssize_t {
            receivedData.append(static_cast<const char*>(buf));
            return receivedData.size();
        }
    ));
    EXPECT_CALL(stdLib, close(_)).WillOnce(Return(SUCCESS));

    try {
        conn.acceptConnection(listenFd);
        EXPECT_TRUE(conn.writeData(msg));
        EXPECT_STREQ(msg.c_str(), receivedData.c_str());
    } catch (const std::exception& e) {
        std::cout << __PRETTY_FUNCTION__ << ":" << __LINE__ << ": " << e.what() << std::endl;
        EXPECT_FALSE(true);
    }
}

TEST_F(ConnectionSocketHelper_tests, writeData_not_connected) {
    ConnectionSocketHelper conn{};
    try {
        EXPECT_FALSE(conn.writeData("abcd"));
    } catch (const socket_error& e) {
        std::cout << __PRETTY_FUNCTION__ << ":" << __LINE__ << ": " << e.what() << std::endl;
        EXPECT_TRUE(true);
    } catch (const std::exception& e) {
        std::cout << __PRETTY_FUNCTION__ << ":" << __LINE__ << ": " << e.what() << std::endl;
        EXPECT_FALSE(true);
    }
}

} // anonymous