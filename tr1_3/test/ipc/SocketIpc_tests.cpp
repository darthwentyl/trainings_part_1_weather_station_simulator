#include <gtest/gtest.h>

#include <mocks/StdLibStaticMock.h>

#include <ipc/SocketIpc.h>
#include <exceptions/socket_error.h>

namespace {

using namespace testing;
using namespace mw::mocks;
using namespace mw::ipc;
using namespace mw::exceptions;

constexpr const int FAILURE = -1;
constexpr const int SUCCESS = 1;
constexpr const int BACKLOG = 1;
constexpr const int PORT = 12345;

class SocketIpc_tests : public Test {
public:
    ~SocketIpc_tests() = default;

protected:
    void setSuccessListening(const int listenFd) {
        auto& stdLib = StdLibStaticMock::get();
        EXPECT_CALL(stdLib, socket(_, _, _)).WillOnce(Return(listenFd));
        EXPECT_CALL(stdLib, setsockopt(_, _, _, _, _)).WillOnce(Return(SUCCESS));
        EXPECT_CALL(stdLib, bind(_, _, _)).WillOnce(Return(SUCCESS));
        EXPECT_CALL(stdLib, listen(_, _)).WillOnce(Return(SUCCESS));
    }
};

TEST_F(SocketIpc_tests, open_success) {
    auto& stdLib = StdLibStaticMock::get();
    const int listenFd = 123;

    EXPECT_CALL(stdLib, socket(Eq(AF_INET), Eq(SOCK_STREAM), Eq(0))).WillOnce(Return(listenFd));
    EXPECT_CALL(stdLib, setsockopt(Eq(listenFd), Eq(SOL_SOCKET), Eq(SO_REUSEADDR), _, _)).WillOnce(Return(SUCCESS));
    EXPECT_CALL(stdLib, bind(Eq(listenFd), _, _)).WillOnce(Return(SUCCESS));
    EXPECT_CALL(stdLib, listen(Eq(listenFd), Eq(BACKLOG))).WillOnce(Return(SUCCESS));
    EXPECT_CALL(stdLib, close(Eq(listenFd))).WillOnce(Return(SUCCESS));

    try {
        auto instance = SocketIpc{PORT};
        instance.open();
        EXPECT_TRUE(true);
    } catch (const std::exception& e) {
        std::cout << __PRETTY_FUNCTION__ << ":" << __LINE__ << ": " << e.what() << std::endl;
        EXPECT_FALSE(true);
    }
}

TEST_F(SocketIpc_tests, open_has_already_opened) {
    auto& stdLib = StdLibStaticMock::get();
    const int listenFd = 123;

    EXPECT_CALL(stdLib, socket(Eq(AF_INET), Eq(SOCK_STREAM), Eq(0))).WillOnce(Return(listenFd));
    EXPECT_CALL(stdLib, setsockopt(Eq(listenFd), Eq(SOL_SOCKET), Eq(SO_REUSEADDR), _, _)).WillOnce(Return(SUCCESS));
    EXPECT_CALL(stdLib, bind(Eq(listenFd), _, _)).WillOnce(Return(SUCCESS));
    EXPECT_CALL(stdLib, listen(Eq(listenFd), Eq(BACKLOG))).WillOnce(Return(SUCCESS));
    EXPECT_CALL(stdLib, close(Eq(listenFd))).WillOnce(Return(SUCCESS));

    try {
        auto instance = SocketIpc{PORT};
        instance.open();
        instance.open();
        EXPECT_TRUE(true);
    } catch (const std::exception& e) {
        std::cout << __PRETTY_FUNCTION__ << ":" << __LINE__ << ": " << e.what() << std::endl;
        EXPECT_FALSE(true);
    }
}

TEST_F(SocketIpc_tests, open_socket_failure) {
    auto& stdLib = StdLibStaticMock::get();

    EXPECT_CALL(stdLib, socket(Eq(AF_INET), Eq(SOCK_STREAM), Eq(0))).WillOnce(Return(FAILURE));

    try {
        auto instance = SocketIpc{PORT};
        instance.open();
        EXPECT_FALSE(true);
    } catch (const socket_error& e) {
        std::cout << __PRETTY_FUNCTION__ << ":" << __LINE__ << ": " << e.what() << std::endl;
        EXPECT_TRUE(true);
    } catch (const std::exception& e) {
        std::cout << __PRETTY_FUNCTION__ << ":" << __LINE__ << ": " << e.what() << std::endl;
        EXPECT_FALSE(true);
    }
}

TEST_F(SocketIpc_tests, open_setsockopt_failure) {
    auto& stdLib = StdLibStaticMock::get();
    const int listenFd = 123;

    EXPECT_CALL(stdLib, socket(Eq(AF_INET), Eq(SOCK_STREAM), Eq(0))).WillOnce(Return(listenFd));
    EXPECT_CALL(stdLib, setsockopt(Eq(listenFd), Eq(SOL_SOCKET), Eq(SO_REUSEADDR), _, _)).WillOnce(Return(FAILURE));
    EXPECT_CALL(stdLib, close(Eq(listenFd))).WillOnce(Return(SUCCESS));

    try {
        auto instance = SocketIpc{PORT};
        instance.open();
        EXPECT_FALSE(true);
    } catch (const socket_error& e) {
        std::cout << __PRETTY_FUNCTION__ << ":" << __LINE__ << ": " << e.what() << std::endl;
        EXPECT_TRUE(true);
    } catch (const std::exception& e) {
        std::cout << __PRETTY_FUNCTION__ << ":" << __LINE__ << ": " << e.what() << std::endl;
        EXPECT_FALSE(true);
    }
}

TEST_F(SocketIpc_tests, open_bind_failure) {
    auto& stdLib = StdLibStaticMock::get();
    const int listenFd = 123;

    EXPECT_CALL(stdLib, socket(Eq(AF_INET), Eq(SOCK_STREAM), Eq(0))).WillOnce(Return(listenFd));
    EXPECT_CALL(stdLib, setsockopt(Eq(listenFd), Eq(SOL_SOCKET), Eq(SO_REUSEADDR), _, _)).WillOnce(Return(SUCCESS));
    EXPECT_CALL(stdLib, bind(Eq(listenFd), _, _)).WillOnce(Return(FAILURE));
    EXPECT_CALL(stdLib, close(Eq(listenFd))).WillOnce(Return(SUCCESS));

    try {
        auto instance = SocketIpc{PORT};
        instance.open();
        EXPECT_FALSE(true);
    } catch (const socket_error& e) {
        std::cout << __PRETTY_FUNCTION__ << ":" << __LINE__ << ": " << e.what() << std::endl;
        EXPECT_TRUE(true);
    } catch (const std::exception& e) {
        std::cout << __PRETTY_FUNCTION__ << ":" << __LINE__ << ": " << e.what() << std::endl;
        EXPECT_FALSE(true);
    }
}

TEST_F(SocketIpc_tests, open_listen_failure) {
    auto& stdLib = StdLibStaticMock::get();
    const int listenFd = 123;

    EXPECT_CALL(stdLib, socket(Eq(AF_INET), Eq(SOCK_STREAM), Eq(0))).WillOnce(Return(listenFd));
    EXPECT_CALL(stdLib, setsockopt(Eq(listenFd), Eq(SOL_SOCKET), Eq(SO_REUSEADDR), _, _)).WillOnce(Return(SUCCESS));
    EXPECT_CALL(stdLib, bind(Eq(listenFd), _, _)).WillOnce(Return(SUCCESS));
    EXPECT_CALL(stdLib, listen(Eq(listenFd), Eq(BACKLOG))).WillOnce(Return(FAILURE));
    EXPECT_CALL(stdLib, close(Eq(listenFd))).WillOnce(Return(SUCCESS));

    try {
        auto instance = SocketIpc{PORT};
        instance.open();
        EXPECT_FALSE(true);
    } catch (const socket_error& e) {
        std::cout << __PRETTY_FUNCTION__ << ":" << __LINE__ << ": " << e.what() << std::endl;
        EXPECT_TRUE(true);
    } catch (const std::exception& e) {
        std::cout << __PRETTY_FUNCTION__ << ":" << __LINE__ << ": " << e.what() << std::endl;
        EXPECT_FALSE(true);
    }
}

TEST_F(SocketIpc_tests, close_success) {
    auto& stdLib = StdLibStaticMock::get();
    const int listenFd = 123;

    setSuccessListening(listenFd);
    EXPECT_CALL(stdLib, close(Eq(listenFd))).WillOnce(Return(SUCCESS));

    try {
        auto instance = SocketIpc{PORT};
        instance.open();
        instance.close();
        EXPECT_TRUE(true);
    } catch (const std::exception& e) {
        std::cout << __PRETTY_FUNCTION__ << ":" << __LINE__ << ": " << e.what() << std::endl;
        EXPECT_FALSE(true);
    }
}

TEST_F(SocketIpc_tests, close_failed) {
    auto& stdLib = StdLibStaticMock::get();
    const int listenFd = 123;

    setSuccessListening(listenFd);
    EXPECT_CALL(stdLib, close(Eq(listenFd))).WillOnce(Return(FAILURE));

    try {
        auto instance = SocketIpc{PORT};
        instance.open();
        instance.close();
        EXPECT_FALSE(true);
    } catch (const socket_error& e) {
        std::cout << __PRETTY_FUNCTION__ << ":" << __LINE__ << ": " << e.what() << std::endl;
        EXPECT_TRUE(true);
    } catch (const std::exception& e) {
        std::cout << __PRETTY_FUNCTION__ << ":" << __LINE__ << ": " << e.what() << std::endl;
        EXPECT_FALSE(true);
    }
}

TEST_F(SocketIpc_tests, close_when_it_is_not_connected) {
    auto& stdLib = StdLibStaticMock::get();
    const int listenFd = 123;

    EXPECT_CALL(stdLib, close(Eq(listenFd))).Times(0);

    try {
        auto instance = SocketIpc{PORT};
        instance.close();
        EXPECT_TRUE(true);
    } catch (const std::exception& e) {
        std::cout << __PRETTY_FUNCTION__ << ":" << __LINE__ << ": " << e.what() << std::endl;
        EXPECT_FALSE(true);
    }
}

TEST_F(SocketIpc_tests, close_destructor_catch_error) {
    auto& stdLib = StdLibStaticMock::get();
    const int listenFd = 123;

    setSuccessListening(listenFd);
    EXPECT_CALL(stdLib, close(Eq(listenFd))).WillOnce(Return(FAILURE));

    try {
        auto instance = SocketIpc{PORT};
        instance.open();
        EXPECT_TRUE(true);
    } catch (const std::exception& e) {
        std::cout << __PRETTY_FUNCTION__ << ":" << __LINE__ << ": " << e.what() << std::endl;
        EXPECT_FALSE(true);
    }
}

TEST_F(SocketIpc_tests, read_success) {
    auto& stdLib = StdLibStaticMock::get();
    const int listenFd = 123;
    const int connectFd = 321;
    const std::string msg = "message\n\r";

    setSuccessListening(listenFd);
    EXPECT_CALL(stdLib, accept(_, _, _)).WillOnce(Return(connectFd));
    EXPECT_CALL(stdLib, read(Eq(connectFd), _, _))
    .WillOnce(
        Invoke([=](int, void* buf, size_t) -> ssize_t {
            strncpy(static_cast<char*>(buf), msg.c_str(), msg.size());
            return msg.size();
        }
    ));
    EXPECT_CALL(stdLib, close(_)).Times(2).WillRepeatedly(Return(SUCCESS));

    try {
        auto instance = SocketIpc{PORT};
        instance.open();
        EXPECT_STREQ(instance.read().c_str(), "message");
    } catch (const std::exception& e) {
        std::cout << __PRETTY_FUNCTION__ << ":" << __LINE__ << ": " << e.what() << std::endl;
        EXPECT_FALSE(true);
    }
}

TEST_F(SocketIpc_tests, read_socket_is_not_opened) {
    try {
        auto instance = SocketIpc{PORT};
        instance.read();
        EXPECT_FALSE(true);
    } catch (const socket_error& e) {
        std::cout << __PRETTY_FUNCTION__ << ":" << __LINE__ << ": " << e.what() << std::endl;
        EXPECT_TRUE(true);
    } catch (const std::exception& e) {
        std::cout << __PRETTY_FUNCTION__ << ":" << __LINE__ << ": " << e.what() << std::endl;
        EXPECT_FALSE(true);
    }
}

TEST_F(SocketIpc_tests, read_success_two_times) {
    auto& stdLib = StdLibStaticMock::get();
    const int listenFd = 123;
    const int connectFd = 321;
    const std::string msg = "message\n\r";

    auto read_msg = [msg](int, void* buf, size_t) -> ssize_t {
        strncpy(static_cast<char*>(buf), msg.c_str(), msg.size());
        return msg.size();
    };

    setSuccessListening(listenFd);
    EXPECT_CALL(stdLib, accept(_, _, _)).WillOnce(Return(connectFd));
    EXPECT_CALL(stdLib, read(Eq(connectFd), _, _)).Times(2).WillRepeatedly(Invoke(read_msg));
    EXPECT_CALL(stdLib, close(_)).Times(2).WillRepeatedly(Return(SUCCESS));

    try {
        auto instance = SocketIpc{PORT};
        instance.open();
        EXPECT_STREQ(instance.read().c_str(), "message");
        EXPECT_STREQ(instance.read().c_str(), "message");
    } catch (const std::exception& e) {
        std::cout << __PRETTY_FUNCTION__ << ":" << __LINE__ << ": " << e.what() << std::endl;
        EXPECT_FALSE(true);
    }
}

TEST_F(SocketIpc_tests, read_client_disconnected) {
    auto& stdLib = StdLibStaticMock::get();
    const int listenFd = 123;
    const int connectFd = 321;

    setSuccessListening(listenFd);
    EXPECT_CALL(stdLib, accept(_, _, _)).WillOnce(Return(connectFd));
    EXPECT_CALL(stdLib, read(Eq(connectFd), _, _)).WillOnce(Return(0));
    EXPECT_CALL(stdLib, close(_)).Times(2).WillRepeatedly(Return(SUCCESS));

    try {
        auto instance = SocketIpc{PORT};
        instance.open();
        EXPECT_STREQ(instance.read().c_str(), "exit");
    } catch (const std::exception& e) {
        std::cout << __PRETTY_FUNCTION__ << ":" << __LINE__ << ": " << e.what() << std::endl;
        EXPECT_FALSE(true);
    }
}

TEST_F(SocketIpc_tests, write_success) {
    auto& stdLib = StdLibStaticMock::get();
    const int listenFd = 123;
    const int connectFd = 321;
    const std::string msg = "abcd";
    std::string receivedData;

    setSuccessListening(listenFd);
    EXPECT_CALL(stdLib, accept(_, _, _)).WillOnce(Return(connectFd));
    EXPECT_CALL(stdLib, write(Eq(connectFd), _, _))
    .WillOnce(
        Invoke([&](int, const void* buf, size_t) -> ssize_t {
            receivedData.append(static_cast<const char*>(buf));
            return receivedData.size();
        }
    ));
    EXPECT_CALL(stdLib, close(_)).Times(2).WillRepeatedly(Return(SUCCESS));

    try {
        auto instance = SocketIpc{PORT};
        instance.open();
        EXPECT_TRUE(instance.write(msg));
        EXPECT_STREQ(msg.c_str(), receivedData.c_str());
    } catch (const std::exception& e) {
        std::cout << __PRETTY_FUNCTION__ << ":" << __LINE__ << ": " << e.what() << std::endl;
        EXPECT_FALSE(true);
    }
}

TEST_F(SocketIpc_tests, write_socket_is_not_opened) {
    try {
        auto instance = SocketIpc{PORT};
        instance.write("message");
        EXPECT_FALSE(true);
    } catch (const socket_error& e) {
        std::cout << __PRETTY_FUNCTION__ << ":" << __LINE__ << ": " << e.what() << std::endl;
        EXPECT_TRUE(true);
    } catch (const std::exception& e) {
        std::cout << __PRETTY_FUNCTION__ << ":" << __LINE__ << ": " << e.what() << std::endl;
        EXPECT_FALSE(true);
    }
}

} // anonymous