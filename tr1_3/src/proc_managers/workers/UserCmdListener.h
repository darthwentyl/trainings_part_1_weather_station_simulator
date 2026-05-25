#pragma once

#include <proc_managers/workers/WeatherData.h>
#include <helpers/FixedDataContainer.h>

namespace mw { namespace proc_managers { namespace workers {

class UserCmdListener {
public:
    UserCmdListener(const std::size_t bufferSize);
    ~UserCmdListener() = default;

    UserCmdListener(const UserCmdListener&) = delete;
    UserCmdListener& operator = (const UserCmdListener&) = delete;
    UserCmdListener(UserCmdListener&&) = delete;
    UserCmdListener& operator = (UserCmdListener&&) = delete;

public:
    helpers::FixedDataContainer<WeatherData> weatherDatas;
};

} } } // mw::proc_managers::workers