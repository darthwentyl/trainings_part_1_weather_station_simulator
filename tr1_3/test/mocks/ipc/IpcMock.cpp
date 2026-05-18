#include <mocks/ipc/IpcMock.h>
#include <gtest/gtest.h>

namespace mw { namespace mocks {

using namespace testing;

IpcMock::IpcMock() {
    ON_CALL(*this, readData()).WillByDefault(Return(std::string{}));
    ON_CALL(*this, writeData(_)).WillByDefault(Return(false));
}

} } // mw::mocks