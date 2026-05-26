#pragma once

#include <proc_managers/workers/handlers/IUserCmdHandler.h>

namespace mw { namespace proc_managers { namespace workers { namespace handlers {

class UserCmdExitHandler : public IUserCmdHandler {
public:
    ~UserCmdExitHandler() = default;

    bool handle(const std::string& command) const override;
};

} } } } // mw::proc_managers::workers::handlers