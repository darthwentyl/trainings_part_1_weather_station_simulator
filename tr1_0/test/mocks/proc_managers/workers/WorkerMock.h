#include <gmock/gmock.h>

#include <proc_managers/workers/IWorker.h>

namespace mw { namespace mocks {

class WorkerMock : public proc_managers::workers::IWorker {
public:
    WorkerMock();
    ~WorkerMock() = default;

    MOCK_METHOD(void, startWorking, (), (override));
    MOCK_METHOD(void, processData, (), (override));
    MOCK_METHOD(void, stopWorking, (), (override));
    MOCK_METHOD(bool, isWorking, (), (override));
};

} } // mw::mocks