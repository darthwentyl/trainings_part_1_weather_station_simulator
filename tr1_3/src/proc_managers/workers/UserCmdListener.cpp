#include <proc_managers/workers/UserCmdListener.h>

namespace mw { namespace proc_managers { namespace workers {

UserCmdListener::UserCmdListener(const std::size_t bufferSize) :
    weatherDatas{bufferSize}
{}

} } } // mw::proc_managers::workers