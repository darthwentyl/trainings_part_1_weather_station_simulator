#include <helpers/GnuplotDescription.h>

namespace mw { namespace helpers {

GnuplotDescription::GnuplotDescription() :
    width{0},
    height{0},
    title{},
    xLabel{},
    yLabel{},
    legend{}
{}

void GnuplotDescription::setWidth(const std::size_t width) {
    this->width = width;
}

void GnuplotDescription::setHeight(const std::size_t height) {
    this->height = height;
}

void GnuplotDescription::setTitle(const std::string& title) {
    this->title = title;
}

void GnuplotDescription::setAxisLabel(const EGnuplotAxis axis, const std::string& label) {
    switch (axis) {
        case EGnuplotAxis::OY:
            this->yLabel = label;
            break;
        case EGnuplotAxis::OX:
        default:
            this->xLabel = label;
    }
}
void GnuplotDescription::setLegend(const std::string& legend) {
    this->legend = legend;
}

void GnuplotDescription::setDataFile(const std::string& dataFile) {
    this->dataFile = dataFile;
}

const std::size_t& GnuplotDescription::getWidth() const {
    return width;
}

const std::size_t& GnuplotDescription::getHeight() const {
    return height;
}

const std::string& GnuplotDescription::getTitle() const {
    return title;
}

const std::string& GnuplotDescription::getAxisLabel(const EGnuplotAxis axis) const {
    switch (axis) {
        case EGnuplotAxis::OY: return yLabel;
        case EGnuplotAxis::OX:
        default:
            return xLabel;
    }
}

const std::string& GnuplotDescription::getLegend() const {
    return legend;
}

const std::string& GnuplotDescription::getDataFile() const {
    return  dataFile;
}

} } // mw::helpers