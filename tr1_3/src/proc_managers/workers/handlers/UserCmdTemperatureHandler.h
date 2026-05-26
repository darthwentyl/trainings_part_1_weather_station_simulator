#pragma once

#include <proc_managers/workers/handlers/IUserCmdHandler.h>
#include <proc_managers/workers/WeatherData.h>
#include <helpers/FixedDataContainer.h>

namespace mw { namespace ipc {

    class IIpc;

} } // mw::ipc

namespace mw { namespace proc_managers { namespace workers { namespace handlers {

class UserCmdTemperatureHandler : public IUserCmdHandler {
public:
    UserCmdTemperatureHandler(ipc::IIpc& ipc, helpers::FixedDataContainer<WeatherData>& weatherDatas);
    ~UserCmdTemperatureHandler() = default;

    bool handle(const std::string& command) const override;

private:
    std::size_t getNLastElemToRead(const std::string& command) const;

    ipc::IIpc& ipc;
    helpers::FixedDataContainer<WeatherData>& weatherDatas;

};

} } } } // mw::proc_managers::workers::handlers