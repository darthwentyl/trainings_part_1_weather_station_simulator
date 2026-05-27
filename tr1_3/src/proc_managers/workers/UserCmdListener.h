#pragma once

#include <proc_managers/workers/WeatherData.h>
#include <proc_managers/workers/handlers/IUserCmdHandler.h>
#include <helpers/FixedDataContainer.h>

#include <thread>
#include <mutex>
#include <stop_token>
#include <unordered_map>
#include <memory>

namespace mw { namespace ipc {

    class IIpc;

} } // mw::ipc

namespace mw { namespace proc_managers { namespace workers {

class UserCmdListener {
public:
    UserCmdListener(ipc::IIpc& ipc, const std::size_t bufferSize) noexcept;
    ~UserCmdListener();

    UserCmdListener(const UserCmdListener&) = delete;
    UserCmdListener& operator = (const UserCmdListener&) = delete;
    UserCmdListener(UserCmdListener&&) = delete;
    UserCmdListener& operator = (UserCmdListener&&) = delete;

    void listen(std::stop_token stopToken);

    void startListening();
    void addWeatherData(const WeatherData& data);
    void stopListening();
    bool isListening() const;

private:
    enum class EHandlerId {
        EXIT,
        TEMPERATURE
    };

    /***************************************************
     * true  - continue listening                      *
     * false - stop listening                          *
     ***************************************************/
    bool handleCommand(const std::string& command);

    ipc::IIpc& ipc;
    helpers::FixedDataContainer<WeatherData> weatherDatas;
    const std::unordered_map<EHandlerId, std::shared_ptr<handlers::IUserCmdHandler>> handlers;
    std::mutex dataMutex;
    std::jthread listenThread;
};

} } } // mw::proc_managers::workers