#include <mocks/StdLibStaticMock.h>
#include <gtest/gtest.h>

namespace mw { namespace mocks {

namespace {
    constexpr int FAILURE = -1;
} // anonymous

using namespace testing;

StrictMock<StdLibStaticMock>& StdLibStaticMock::get() {
    static StrictMock<StdLibStaticMock> instance;
    return instance;
}

StdLibStaticMock::StdLibStaticMock() {
    ON_CALL(*this, ftok(_, _)).WillByDefault(Return(FAILURE));
    ON_CALL(*this, semget(_, _, _)).WillByDefault(Return(FAILURE));
    ON_CALL(*this, semctl(_, _, _)).WillByDefault(Return(FAILURE));
    ON_CALL(*this, semop(_, _, _)).WillByDefault(Return(FAILURE));
    ON_CALL(*this, getpid()).WillByDefault(Return(FAILURE));
    ON_CALL(*this, shmget(_, _, _)).WillByDefault(Return(FAILURE));
    ON_CALL(*this, shmat(_, _, _)).WillByDefault(Return(nullptr));
    ON_CALL(*this, shmctl(_, _, _)).WillByDefault(Return(FAILURE));
    ON_CALL(*this, shmdt(_)).WillByDefault(Return(FAILURE));
    ON_CALL(*this, popen(_, _)).WillByDefault(Return(nullptr));
    ON_CALL(*this, pclose(_)).WillByDefault(Return(FAILURE));
    ON_CALL(*this, fgets(_, _, _)).WillByDefault(Return(nullptr));
    ON_CALL(*this, fputs(_, _)).WillByDefault(Return(EOF));
}

} } // mw::mocks