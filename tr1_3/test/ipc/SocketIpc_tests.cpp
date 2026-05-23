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


} // anonymous