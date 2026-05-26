#include <proc_managers/workers/handlers/UserCmdExitHandler.h>

#include <logger/Log.h>

namespace mw { namespace proc_managers { namespace workers { namespace handlers {

bool UserCmdExitHandler::handle(const std::string& command) {
    if (command == "exit" || command.empty()) {
        return false; // false means stop listening
    }
    DEBUG("handle incorrect command: " << command);
    return true;
}

} } } } // mw::proc_managers::workers::handlers