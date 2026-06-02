#include <gtest/gtest.h>

#include <ipc/SharedSegmentMessage.h>

namespace {

using namespace testing;
using namespace mw::ipc;

class SharedSegmentMessage_tests : public Test {
public:
    ~SharedSegmentMessage_tests() = default;
};

TEST_F(SharedSegmentMessage_tests, serialize_deserialize_non_exit) {
    SharedSegmentMessage msg{};

    const std::string mem = "Hello world";

    msg.deserialize(mem.c_str());
    EXPECT_STREQ(msg.serialize().c_str(), std::string{mem + std::string{" isExit: 0"}}.c_str());
}

TEST_F(SharedSegmentMessage_tests, serialize_deserialize_exit) {
    SharedSegmentMessage msg{};

    const std::string mem = "exit";

    msg.deserialize(mem.c_str());
    EXPECT_STREQ(msg.serialize().c_str(), mem.c_str());
}

TEST_F(SharedSegmentMessage_tests, serialize_deserialize_exit_in_mem) {
    SharedSegmentMessage msg{};

    const std::string mem = "Hello world isExit: 1";

    msg.deserialize(mem.c_str());
    EXPECT_STREQ(msg.serialize().c_str(), "exit");
}

TEST_F(SharedSegmentMessage_tests, serialize_deserialize_non_exit_in_mem) {
    SharedSegmentMessage msg{};

    const std::string mem = "Hello world isExit: 0";

    msg.deserialize(mem.c_str());
    EXPECT_STREQ(msg.serialize().c_str(), mem.c_str());
}

} // anonymous