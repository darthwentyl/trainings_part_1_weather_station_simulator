#pragma once

#include <gmock/gmock.h>

#include <ipc/IIpc.h>

namespace mocks {

class IpcMock : public mw::ipc::IIpc {
public:
    ~IpcMock() = default;

    MOCK_METHOD(void, open, (), (override));
    MOCK_METHOD(void, close, (), (override));
    MOCK_METHOD(std::string, read, (), (const, override));
    MOCK_METHOD(bool, write, (const std::string& msg), (override));
};

} // mocks