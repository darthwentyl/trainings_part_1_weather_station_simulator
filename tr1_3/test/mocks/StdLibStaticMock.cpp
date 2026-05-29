#include <mocks/StdLibStaticMock.h>
#include <gtest/gtest.h>
#include <dlfcn.h>
#include <unistd.h>
#include <cstddef>
#include <sys/types.h>

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
    ON_CALL(*this, socket(_, _, _)).WillByDefault(Return(FAILURE));
    ON_CALL(*this, setsockopt(_, _, _, _, _)).WillByDefault(Return(FAILURE));
    ON_CALL(*this, bind(_, _, _)).WillByDefault(Return(FAILURE));
    ON_CALL(*this, listen(_, _)).WillByDefault(Return(FAILURE));
    ON_CALL(*this, accept(_, _, _)).WillByDefault(Return(FAILURE));
    ON_CALL(*this, close(_)).WillByDefault(Return(FAILURE));
    ON_CALL(*this, read(_, _, _)).WillByDefault(Invoke(Return(0)));
    ON_CALL(*this, write(_, _, _)).WillByDefault(Return(0));
    ON_CALL(*this, connect(_, _, _)).WillByDefault(Return(FAILURE));
    ON_CALL(*this, send(_, _, _, _)).WillByDefault(Return(0));
}

} } // mw::mocks