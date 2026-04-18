#pragma once

#include <string>

namespace mw { namespace proc_managers { namespace workers {

class WeatherData {
public:
    WeatherData();

    double getTemperature() const;
    double getPressure() const;

    void setTemperature(const double temperature);
    void setPressure(const double pressure);

    std::string serialize();
    void deserialize(const std::string& data);

private:
    double temperature;
    double pressure;
};

} } } // mw::proc_managers::workers