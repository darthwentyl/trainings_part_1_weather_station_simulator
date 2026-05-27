#include <proc_managers/workers/UserCmdListener.h>
#include <proc_managers/workers/handlers/UserCmdExitHandler.h>
#include <proc_managers/workers/handlers/UserCmdTemperatureHandler.h>
#include <ipc/IIpc.h>
#include <logger/Log.h>

#include <sstream>
#include <limits>

namespace {

constexpr const char* EXIT_CMD = "exit";
constexpr const char* TEMPERATURE_CMD = "temperature";
constexpr const char* PRESSURE_CMD = "pressure";
constexpr const char* HELP_CMD = "help";

} // anonymous

namespace mw { namespace proc_managers { namespace workers {

using namespace mw::ipc;
using namespace mw::proc_managers::workers::handlers;

UserCmdListener::UserCmdListener(IIpc& ipc, const std::size_t bufferSize) noexcept :
    ipc{ipc},
    weatherDatas{bufferSize},
    handlers{
        { EHandlerId::EXIT, std::make_shared<UserCmdExitHandler>() },
        { EHandlerId::TEMPERATURE, std::make_shared<UserCmdTemperatureHandler>(ipc, weatherDatas) }
    }
{}

UserCmdListener::~UserCmdListener() {
    DEBUG("");
    stopListening();
}

void UserCmdListener::listen(std::stop_token stopToken) {
    DEBUG("Listening started");
    std::string userCommand;
    while(
         handleCommand(ipc.read()) &&
         !stopToken.stop_requested()
    );
    DEBUG("Listening stopped");
}

void UserCmdListener::startListening() {
    DEBUG("");

    if (listenThread.joinable()) {
        DEBUG("Listener has already started");
        return;
    }
    ipc.open();
    listenThread = std::jthread{&UserCmdListener::listen, std::ref(*this)};
}

void UserCmdListener::addWeatherData(const WeatherData& data) {
    DEBUG("");
    std::unique_lock<std::mutex> lock{dataMutex};
    weatherDatas.pushBack(data);
}

void UserCmdListener::stopListening() {
    DEBUG("");
    if (!listenThread.joinable()) {
        DEBUG("Listener has already stopped");
        return;
    }
    listenThread.request_stop();
    listenThread.join();
    ipc.close();
}

bool UserCmdListener::isListening() const {
    return listenThread.joinable();
}

bool UserCmdListener::handleCommand(const std::string& command) {
    DEBUG("command: " << command);
    if (command.empty() || command == EXIT_CMD) {
        return handlers.at(EHandlerId::EXIT)->handle(command);
    } else if (command.starts_with(TEMPERATURE_CMD)) {
        return handlers.at(EHandlerId::TEMPERATURE)->handle(command);
    } else {
        return true;
    }
}


} } } // mw::proc_managers::workers