#include <gtest/gtest.h>

#include <mocks/StdLibStaticMock.h>

#include <helpers/ClientTelnetHelper.h>
#include <exceptions/socket_error.h>

#include <chrono>

namespace {

using namespace testing;
using namespace mw::mocks;
using namespace mw::helpers;
using namespace mw::exceptions;

using namespace std::chrono_literals;

constexpr const int FAILURE = -1;
constexpr const int NOT_CONNECTED = -2;
constexpr const int SUCCESS = 0;
constexpr const int BUFF_SIZE = 128;
constexpr const int PORT = 11111;

class ClientTelnetHelper_tests : public Test {
public:
    ~ClientTelnetHelper_tests() = default;
};

TEST_F(ClientTelnetHelper_tests, acceptConnection_success) {
    auto& stdLib = StdLibStaticMock::get();
    ClientTelnetHelper conn{PORT};
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

TEST_F(ClientTelnetHelper_tests, acceptConnection_failed) {
    auto& stdLib = StdLibStaticMock::get();
    ClientTelnetHelper conn{PORT};
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

TEST_F(ClientTelnetHelper_tests, closeSocket_destructor_success) {
    auto& stdLib = StdLibStaticMock::get();
    ClientTelnetHelper conn{PORT};
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

TEST_F(ClientTelnetHelper_tests, closeSocket_destructor_failure) {
    auto& stdLib = StdLibStaticMock::get();
    const int listenFd = 123;
    const int connectFd = 321;

    EXPECT_CALL(stdLib, accept(_, _, _)).WillOnce(Return(connectFd));
    EXPECT_CALL(stdLib, close(Eq(connectFd))).WillOnce(Return(FAILURE));
    try {
        ClientTelnetHelper conn{PORT};
        conn.acceptConnection(listenFd);
        EXPECT_TRUE(conn.isConnected());
    } catch (const std::exception& e) {
        std::cout << __PRETTY_FUNCTION__ << ":" << __LINE__ << ": " << e.what() << std::endl;
        EXPECT_FALSE(true);
    }
}

TEST_F(ClientTelnetHelper_tests, closeSocket_success) {
    auto& stdLib = StdLibStaticMock::get();
    ClientTelnetHelper conn{PORT};
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

TEST_F(ClientTelnetHelper_tests, closeSocket_failure) {
    auto& stdLib = StdLibStaticMock::get();
    ClientTelnetHelper conn{PORT};
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

TEST_F(ClientTelnetHelper_tests, readData_success) {
    auto& stdLib = StdLibStaticMock::get();
    ClientTelnetHelper conn{PORT};
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

TEST_F(ClientTelnetHelper_tests, readData_big_buff) {
    auto& stdLib = StdLibStaticMock::get();
    ClientTelnetHelper conn{PORT};
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

TEST_F(ClientTelnetHelper_tests, readData_not_connected) {
    ClientTelnetHelper conn{PORT};
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

TEST_F(ClientTelnetHelper_tests, writeData_success) {
    auto& stdLib = StdLibStaticMock::get();
    ClientTelnetHelper conn{PORT};
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

TEST_F(ClientTelnetHelper_tests, writeData_not_connected) {
    ClientTelnetHelper conn{PORT};
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

TEST_F(ClientTelnetHelper_tests, cancelAccept_success) {
    auto& stdLib = StdLibStaticMock::get();
    ClientTelnetHelper conn{PORT};
    const int listenFd = 111;
    const int clientFd = 222;

    std::mutex mtx;
    std::condition_variable cv;
    std::thread acceptThread;
    std::thread closeThread;
    bool ready = false;

    EXPECT_CALL(stdLib, accept(_, _, _)).WillOnce(
        Invoke(
            [&](int, struct sockaddr*, socklen_t*) {
                std::unique_lock lock{mtx};
                cv.wait(lock, [&] { return ready; } );
                return NOT_CONNECTED;
            }
        ));

    EXPECT_CALL(stdLib, socket(_, _, _)).WillOnce(Return(clientFd));
    EXPECT_CALL(stdLib, connect(Eq(clientFd), _, _)).WillOnce(Return(SUCCESS));
    EXPECT_CALL(stdLib, send(Eq(clientFd), _, Eq(0), _)).WillOnce(Return(0));
    EXPECT_CALL(stdLib, close(Eq(clientFd))).WillOnce(
        Invoke(
            [&](int){
                std::unique_lock lock{mtx};
                ready = true;
                lock.unlock();
                cv.notify_all();
                return SUCCESS;
            }
        ));

    acceptThread = std::thread{&ClientTelnetHelper::acceptConnection, std::ref(conn), listenFd};
    std::this_thread::sleep_for(50ms); // to ensure that acceptThread started;
    closeThread = std::thread{&ClientTelnetHelper::closeConnection, std::ref(conn)};
    acceptThread.join();
    closeThread.join();
}

TEST_F(ClientTelnetHelper_tests, cancelAccept_socket_failed) {
    auto& stdLib = StdLibStaticMock::get();
    ClientTelnetHelper conn{PORT};
    const int listenFd = 111;

    std::mutex mtx;
    std::condition_variable cv;
    std::thread acceptThread;
    std::thread closeThread;
    bool ready = false;

    EXPECT_CALL(stdLib, accept(_, _, _)).WillOnce(
        Invoke(
            [&](int, struct sockaddr*, socklen_t*) {
                std::unique_lock lock{mtx};
                cv.wait(lock, [&] {return ready; });
                return NOT_CONNECTED;
            }
        ));

    EXPECT_CALL(stdLib, socket(_, _, _)).WillOnce(
        Invoke(
            [&](int, int, int) {
                std::unique_lock lock{mtx};
                ready = true;
                lock.unlock();
                cv.notify_all();
                return FAILURE;
            }
        ));

    acceptThread = std::thread{&ClientTelnetHelper::acceptConnection, std::ref(conn), listenFd};
    std::this_thread::sleep_for(50ms); // to ensure that acceptThread started;
    closeThread = std::thread{&ClientTelnetHelper::closeConnection, std::ref(conn)};
    acceptThread.join();
    closeThread.join();
}

TEST_F(ClientTelnetHelper_tests, cancelAccept_connect_failed) {
    auto& stdLib = StdLibStaticMock::get();
    ClientTelnetHelper conn{PORT};
    const int listenFd = 111;
    const int clientFd = 222;

    std::mutex mtx;
    std::condition_variable cv;
    std::thread acceptThread;
    std::thread closeThread;
    bool ready = false;

    EXPECT_CALL(stdLib, accept(_, _, _)).WillOnce(
        Invoke(
            [&](int, struct sockaddr*, socklen_t*) {
                std::unique_lock lock{mtx};
                cv.wait(lock, [&] { return ready; } );
                return NOT_CONNECTED;
            }
        ));
    EXPECT_CALL(stdLib, socket(_, _, _)).WillOnce(Return(clientFd));
    EXPECT_CALL(stdLib, connect(Eq(clientFd), _, _)).WillOnce(
        Invoke(
            [&](int, const struct sockaddr*, socklen_t) {
                std::unique_lock lock{mtx};
                ready = true;
                lock.unlock();
                cv.notify_all();
                return FAILURE;
            }
        ));
    EXPECT_CALL(stdLib, close(Eq(clientFd))).WillOnce(Return(SUCCESS));

    acceptThread = std::thread{&ClientTelnetHelper::acceptConnection, std::ref(conn), listenFd};
    std::this_thread::sleep_for(50ms); // to ensure that acceptThread started;
    closeThread = std::thread{&ClientTelnetHelper::closeConnection, std::ref(conn)};
    acceptThread.join();
    closeThread.join();
}

} // anonymous