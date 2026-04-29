#include <proc_managers/workers/WeatherData.h>

#include <logger/Log.h>

#include <sstream>
#include <cmath>

namespace mw { namespace proc_managers { namespace workers {

constexpr const double MAX_TEMPERATURE = 80.0;
constexpr const double MIN_TEMPERATURE = -40.0;
constexpr const double MAX_PRESSURE = 1050.0;
constexpr const double MIN_PRESSURE = 950.0;
constexpr const double PRECISSION = 0.005;

constexpr const char* TEMPERATURE_NAME = "temperature: ";
constexpr const char* TEMPERATURE_UNIT_NAME = "[C]";
constexpr const char* SEPARATOR = ", ";
constexpr const char* PRESSURE_NAME = "pressure: ";
constexpr const char* PRESSURE_UNIT_NAME = "[hPa]";

WeatherData::WeatherData() :
    temperature{0.0},
    pressure{0.0}
{}

double WeatherData::getTemperature() const {
    return temperature;
}

double WeatherData::getPressure() const {
    return pressure;
}

void WeatherData::setTemperature(const double temperature) {
    if (temperature > MIN_TEMPERATURE && temperature < MAX_TEMPERATURE) {
        double precission =  temperature >= 0.0 ? PRECISSION : -PRECISSION;
        this->temperature = std::trunc((temperature + precission) * 100.0) / 100.0;
    } else {
        DEBUG("temperature " << temperature << " out of range (" << MIN_TEMPERATURE << "; " << MAX_TEMPERATURE << "). Skip it");
    }
}

void WeatherData::setPressure(const double pressure) {
    if (pressure > MIN_PRESSURE && pressure < MAX_PRESSURE) {
        this->pressure = std::trunc((pressure + PRECISSION) * 100.0) / 100.0;
    } else {
        DEBUG("pressure " << pressure << " out of range (" << MIN_PRESSURE << "; " << MAX_PRESSURE << "). Skip it");
    }
}

std::string WeatherData::serialize() {
    std::ostringstream os;
    os << TEMPERATURE_NAME << temperature << TEMPERATURE_UNIT_NAME
       << SEPARATOR
       << PRESSURE_NAME << pressure << PRESSURE_UNIT_NAME;
    return os.str();
}

void WeatherData::deserialize(const std::string& data) {
    std::string skip;
    std::istringstream is(data);
    is >> skip >> temperature >> skip >> skip >> pressure;
}

} } } // mw::proc_managers::workers