#include <proc_managers/workers/handlers/UserCmdPressureHandler.h>
#include <ipc/IIpc.h>
#include <logger/Log.h>

#include <sstream>
#include <format>

namespace mw { namespace proc_managers { namespace workers { namespace handlers {

using namespace mw::helpers;
using namespace mw::ipc;

UserCmdPressureHandler::UserCmdPressureHandler(IIpc& ipc, FixedDataContainer<WeatherData>& weatherDatas) :
    ipc{ipc},
    weatherDatas{weatherDatas}
{}

bool UserCmdPressureHandler::handle(const std::string& command) const {
    DEBUG("command: " << command);

    std::size_t n = getNLastElemToRead(command);
    const std::size_t size = weatherDatas.getItems().size();

    std::ostringstream oss;

    if (size == 0) {
        oss << "No pressure measurement, now" << std::endl;
    } else {
        oss << "Pressure:" << std::endl;
        for (std::size_t i = size; i > (size - n); --i) {
            oss << std::format("{:3}. {:.>10.2f} [C]\n", i, weatherDatas.getItem(i - 1).getPressure());
        }
    }

    DEBUG("msg: " << std::endl << oss.str());

    return ipc.write(oss.str());
}

std::size_t UserCmdPressureHandler::getNLastElemToRead(const std::string& command) const {
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