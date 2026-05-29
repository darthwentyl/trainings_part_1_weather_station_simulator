#pragma once

#include <proc_managers/workers/handlers/IUserCmdHandler.h>

namespace mw { namespace ipc {

    class IIpc;

} } // mw::ipc

namespace mw { namespace proc_managers { namespace workers { namespace handlers {

class UserCmdHelpHandler : public IUserCmdHandler {
public:
    UserCmdHelpHandler(ipc::IIpc& ipc);
    ~UserCmdHelpHandler() = default;

    bool handle(const std::string& command) const override;

private:
    ipc::IIpc& ipc;
};

} } } } // mw::proc_managers::workers::handlers