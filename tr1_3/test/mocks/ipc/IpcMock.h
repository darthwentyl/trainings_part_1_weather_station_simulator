#pragma once

#include <gmock/gmock.h>

#include <ipc/IIpc.h>

namespace mw { namespace mocks {

class IpcMock : public ipc::IIpc {
public:
    IpcMock();
    ~IpcMock() = default;

    MOCK_METHOD(void, open, (), (override));
    MOCK_METHOD(void, close, (), (override));
    MOCK_METHOD(std::string, readData, (), (const, override));
    MOCK_METHOD(bool, writeData, (const std::string& msg), (const, override));
};

} } // mw::mocks