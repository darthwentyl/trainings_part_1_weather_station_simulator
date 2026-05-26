#include <proc_managers/workers/handlers/UserCmdTemperatureHandler.h>
#include <ipc/IIpc.h>
#include <logger/Log.h>

#include <sstream>

namespace mw { namespace proc_managers { namespace workers { namespace handlers {

using namespace mw::helpers;
using namespace mw::ipc;

UserCmdTemperatureHandler::UserCmdTemperatureHandler(IIpc& ipc, FixedDataContainer<WeatherData>& weatherDatas) :
    ipc{ipc},
    weatherDatas{weatherDatas}
{}

bool UserCmdTemperatureHandler::handle(const std::string& command) const {
    DEBUG("command: " << command);

    std::size_t n = getNLastElemToRead(command);
    const std::size_t size = weatherDatas.getItems().size();

    std::ostringstream oss;

    if (size == 0) {
        oss << "No temperature measurement, now" << std::endl;
    } else {
        oss << "Temperature:" << std::endl;
        for (std::size_t i = size; i > (size - n); --i) {
            oss << "\t" << i << ".\t" << weatherDatas.getItem(i - 1).getTemperature() << "[C]" << std::endl;
        }
    }

    DEBUG("msg: " << std::endl << oss.str());

    return ipc.write(oss.str());
}

std::size_t UserCmdTemperatureHandler::getNLastElemToRead(const std::string& command) const {
    const std::size_t maxElems = weatherDatas.getMaxNumItems();
    const std::size_t size = weatherDatas.getItems().size();
    std::size_t nElems = maxElems;

    std::istringstream iss{command};
    std::string skipCommandName;
    iss >> skipCommandName >> nElems;

    if (nElems >= maxElems || nElems >= size) {
        nElems = size;
    }

    return nElems;
}

} } } } // mw::proc_managers::workers::handlers