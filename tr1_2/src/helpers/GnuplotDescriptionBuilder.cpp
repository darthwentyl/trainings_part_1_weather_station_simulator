#include <helpers/GnuplotDescriptionBuilder.h>
#include <logger/Log.h>

namespace mw { namespace helpers {

constexpr const std::size_t MAX_WIDTH = 1000;
constexpr const std::size_t MAX_HEIGHT = 1000;

GnuplotDescriptionBuilder& GnuplotDescriptionBuilder::width(const std::size_t width) {
    if (width <= MAX_WIDTH) {
        gnuplotDescription.setWidth(width);
    } else {
        INFO("width " << width << " is bigger than max width " << MAX_WIDTH);
    }
    return *this;
}

GnuplotDescriptionBuilder& GnuplotDescriptionBuilder::height(const std::size_t height) {
    if (height <= MAX_HEIGHT) {
        gnuplotDescription.setHeight(height);
    } else {
        INFO("height " << height << " is bigger than max height " << MAX_HEIGHT);
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

GnuplotDescription GnuplotDescriptionBuilder::build() {
    return gnuplotDescription;
}

} } // mw::helpers