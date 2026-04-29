#include <mocks/ipc/IpcMock.h>
#include <gtest/gtest.h>

namespace mw { namespace mocks {

using namespace testing;

IpcMock::IpcMock() {
    ON_CALL(*this, read()).WillByDefault(Return(std::string{}));
}

} } // mw::mocks