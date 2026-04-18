#include <proc_managers/workers/Worker.h>

namespace mw { namespace proc_managers { namespace workers {

Worker::Worker(const bool working) :
    working{working}
{}

bool Worker::isWorking() {
    return  working;
}

void Worker::setWorkingState(const bool working) {
    this->working = working;
}

} } } // mw::proc_managers::workers