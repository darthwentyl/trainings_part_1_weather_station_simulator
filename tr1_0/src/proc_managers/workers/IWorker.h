#pragma once

#include <string>

namespace mw { namespace proc_managers { namespace workers {

class IWorker {
public:
    virtual ~IWorker() = default;

    virtual void startWorking() = 0;
    virtual void processData() = 0;
    virtual void stopWorking() = 0;
    virtual bool isWorking() = 0;
};

} } } // mw::proc_managers::workers