#include <proc_managers/workers/handlers/UserCmdHelpHandler.h>
#include <ipc/IIpc.h>
#include <logger/Log.h>

#include <sstream>

namespace mw { namespace proc_managers { namespace workers { namespace handlers {

using namespace mw::ipc;

UserCmdHelpHandler::UserCmdHelpHandler(IIpc& ipc) :
    ipc{ipc}
{}

bool UserCmdHelpHandler::handle(const std::string& command) const {
    DEBUG("command: " << command);

    std::ostringstream oss;
    if (command != "help") {
        oss << "Wrong command: " << command << std::endl;
    }

    oss << "Available command: " << std::endl
        << "\ttemperature    \t- return last 10 temperature measurements" << std::endl
        << "\ttemperature <n>\t- return last n temperature measurements" << std::endl
        << "\texit           \t- disconnect from application - enter also disconnect" << std::endl
        << "\thelp           \t- show this help" << std::endl;

    return ipc.write(oss.str());
}

} } } } // mw::proc_managers::workers::handlers