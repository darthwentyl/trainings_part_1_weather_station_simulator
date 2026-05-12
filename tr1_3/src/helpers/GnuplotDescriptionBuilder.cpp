#include <helpers/GnuplotDescriptionBuilder.h>
#include <logger/Log.h>

namespace mw { namespace helpers {

constexpr const std::size_t MAX_WIDTH = 1000;
constexpr const std::size_t DEFAULT_WIDTH = 400;
constexpr const std::size_t MIN_WIDTH = 100;

constexpr const std::size_t MAX_HEIGHT = 1000;
constexpr const std::size_t DEFAULT_HEIGHT = 600;
constexpr const std::size_t MIN_HEIGHT = 100;


GnuplotDescriptionBuilder& GnuplotDescriptionBuilder::width(const std::size_t width) {
    if (width <= MAX_WIDTH && width >= MIN_WIDTH) {
        gnuplotDescription.setWidth(width);
    } else {
        INFO("width " << width << " is out of range <" << MIN_WIDTH << "; " << MAX_WIDTH << "> set default " << DEFAULT_WIDTH);
        gnuplotDescription.setWidth(DEFAULT_WIDTH);
    }
    return *this;
}

GnuplotDescriptionBuilder& GnuplotDescriptionBuilder::height(const std::size_t height) {
    if (height <= MAX_HEIGHT && height >= MIN_HEIGHT) {
        gnuplotDescription.setHeight(height);
    } else {
        INFO("height " << height << " is out of range <" << MIN_HEIGHT << "; " << MAX_HEIGHT << "> set default " << DEFAULT_HEIGHT);
        gnuplotDescription.setHeight(DEFAULT_HEIGHT);
    }
    return *this;
}

GnuplotDescriptionBuilder& GnuplotDescriptionBuilder::title(const std::string& title) {
    gnuplotDescription.setTitle(title);
    return *this;
}

GnuplotDescriptionBuilder& GnuplotDescriptionBuilder::xLabel(const std::string& xLabel) {
    gnuplotDescription.setAxisLabel(EGnuplotAxis::OX, xLabel);
    return *this;
}

GnuplotDescriptionBuilder& GnuplotDescriptionBuilder::yLabel(const std::string& yLabel) {
    gnuplotDescription.setAxisLabel(EGnuplotAxis::OY, yLabel);
    return *this;
}

GnuplotDescriptionBuilder& GnuplotDescriptionBuilder::legend(const std::string& legend) {
   gnuplotDescription.setLegend(legend);
   return *this;
}

GnuplotDescriptionBuilder& GnuplotDescriptionBuilder::dataFile(const std::string& dataFile) {
    gnuplotDescription.setDataFile(dataFile);
    return *this;
}

GnuplotDescriptionBuilder& GnuplotDescriptionBuilder::grid(const bool grid) {
    gnuplotDescription.setGrid(grid);
    return *this;
}

GnuplotDescription GnuplotDescriptionBuilder::build() {
    return gnuplotDescription;
}

} } // mw::helpers