#include <gmock/gmock.h>

#include <ipc/ISemaphoreIpc.h>

namespace mw { namespace mocks {

class SemaphoreIpcMock : public ipc::ISemaphoreIpc {
public:
    ~SemaphoreIpcMock() = default;

    MOCK_METHOD(void, open, (const std::size_t initial_val), (override));
    MOCK_METHOD(void, wait, (), (override));
    MOCK_METHOD(void, post, (), (override));
    MOCK_METHOD(void, close, (), (override));
};

} } // mw::mocks
