#include <mocks/proc_managers/workers/WorkerMock.h>
#include <gtest/gtest.h>

namespace mw { namespace mocks {

using namespace testing;

WorkerMock::WorkerMock() {
    ON_CALL(*this, isWorking()).WillByDefault(Return(false));
}

} } // mw::mocks