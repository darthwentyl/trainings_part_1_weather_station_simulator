#pragma once

#include <proc_managers/workers/IWorker.h>

namespace mw { namespace proc_managers { namespace workers {

class Worker : public  IWorker {
public:
    Worker(const bool working);
    ~Worker() = default;

    bool isWorking() override;

protected:
    void setWorkingState(const bool working);

private:
    bool working;
};

} } }  // mw::proc_managers::workes