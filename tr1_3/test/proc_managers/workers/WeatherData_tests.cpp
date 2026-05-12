#include <gtest/gtest.h>
#include <proc_managers/workers/WeatherData.h>

namespace {

using namespace testing;
using namespace mw::proc_managers::workers;

// taken from WeatherData.cpp
constexpr const double MAX_TEMPERATURE = 80.0;
constexpr const double MIN_TEMPERATURE = -40.0;
constexpr const double MAX_PRESSURE = 1050.0;
constexpr const double MIN_PRESSURE = 950.0;

class WeatherData_tests : public Test {
public:
    ~WeatherData_tests() = default;

protected:
    void TearDown() override {
        data = WeatherData{};
    }

    WeatherData data;
};

TEST_F(WeatherData_tests, constructor) {
    EXPECT_DOUBLE_EQ(0.0, data.getTemperature());
    EXPECT_DOUBLE_EQ(0.0, data.getPressure());
}

TEST_F(WeatherData_tests, set_get_valid_data) {
    const double temperature = 20.0;
    const double pressure = 1010.2;

    data.setTemperature(temperature);
    data.setPressure(pressure);

    EXPECT_DOUBLE_EQ(temperature, data.getTemperature());
    EXPECT_DOUBLE_EQ(pressure, data.getPressure());
}

TEST_F(WeatherData_tests, set_get_too_high) {
    const double temperature = 20.0;
    const double pressure = 1010.2;

    // set valid weather data
    data.setTemperature(temperature);
    data.setPressure(pressure);
    // set too high weather data
    data.setTemperature(MAX_TEMPERATURE);
    data.setPressure(MAX_PRESSURE);

    EXPECT_DOUBLE_EQ(temperature, data.getTemperature());
    EXPECT_DOUBLE_EQ(pressure, data.getPressure());
}

TEST_F(WeatherData_tests, set_get_too_low) {
    const double temperature = -20.0;
    const double pressure = 1010.2;

    // set valid weather data
    data.setTemperature(temperature);
    data.setPressure(pressure);
    // set too low weather data
    data.setTemperature(MIN_TEMPERATURE);
    data.setPressure(MIN_PRESSURE);

    EXPECT_DOUBLE_EQ(temperature, data.getTemperature());
    EXPECT_DOUBLE_EQ(pressure, data.getPressure());
}

TEST_F(WeatherData_tests, set_get_2_digits_places) {
    const double temperature = 20.1234;
    const double precission_temp = 20.12;
    const double pressure = 1010.1234;
    const double precission_press = 1010.12;

    data.setTemperature(temperature);
    data.setPressure(pressure);

    EXPECT_DOUBLE_EQ(precission_temp, data.getTemperature());
    EXPECT_DOUBLE_EQ(precission_press, data.getPressure());
}

TEST_F(WeatherData_tests, set_get_2_digits_places_rounding_up) {
    const double temperature = 20.1256;
    const double precission_temp = 20.13;
    const double pressure = 1010.1256;
    const double precission_press = 1010.13;

    data.setTemperature(temperature);
    data.setPressure(pressure);

    EXPECT_DOUBLE_EQ(precission_temp, data.getTemperature());
    EXPECT_DOUBLE_EQ(precission_press, data.getPressure());
}

TEST_F(WeatherData_tests, set_get_2_digits_places_rounding_up_temp_below_0) {
    const double temperature = -20.1256;
    const double precission_temp = -20.13;

    data.setTemperature(temperature);

    EXPECT_DOUBLE_EQ(precission_temp, data.getTemperature());
}

TEST_F(WeatherData_tests, serialize_temp_above_0) {
    const double temperature = 12.567;
    const double pressure = 1000.567;
    const std::string serialized_data = "temperature: 12.57[C], pressure: 1000.57[hPa]";

    data.setTemperature(temperature);
    data.setPressure(pressure);

    EXPECT_STREQ(data.serialize().c_str(), serialized_data.c_str());
}

TEST_F(WeatherData_tests, serialize_temp_below_0) {
    const double temperature = -12.567;
    const double pressure = 1000.567;
    const std::string serialized_data = "temperature: -12.57[C], pressure: 1000.57[hPa]";

    data.setTemperature(temperature);
    data.setPressure(pressure);

    EXPECT_STREQ(data.serialize().c_str(), serialized_data.c_str());
}

TEST_F(WeatherData_tests, deserialize_temp_above_0) {
    const double temperature = 24.57;
    const double pressure = 996.57;
    const std::string serialized_data = "temperature: 24.57[C], pressure: 996.57[hPa]";

    data.deserialize(serialized_data);

    EXPECT_DOUBLE_EQ(temperature, data.getTemperature());
    EXPECT_DOUBLE_EQ(pressure, data.getPressure());
}

TEST_F(WeatherData_tests, deserialize_temp_below_0) {
    const double temperature = -12.99;
    const double pressure = 1013.34;
    const std::string serialized_data = "temperature: -12.99[C], pressure: 1013.34[hPa]";

    data.deserialize(serialized_data);

    EXPECT_DOUBLE_EQ(temperature, data.getTemperature());
    EXPECT_DOUBLE_EQ(pressure,  data.getPressure());
}

} // anonymous