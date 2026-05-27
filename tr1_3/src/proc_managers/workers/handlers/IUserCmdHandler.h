#pragma once

#include <string>

namespace mw { namespace proc_managers { namespace workers { namespace handlers {

class IUserCmdHandler {
public:
    virtual ~IUserCmdHandler() = default;

    /*****************************************************************
     * false - when the command inform about exit or disconnect user *
     * true - otherwise                                              *
     *****************************************************************/
    virtual bool handle(const std::string& command) const = 0;
};

} } } } // mw::proc_managers::workers::handlers