#include <helpers/GnuplotDescription.h>

namespace mw { namespace helpers {

GnuplotDescription::GnuplotDescription(const std::size_t width,
    const std::size_t height,
    const std::string& title,
    const std::string& xLabel,
    const std::string& yLabel) :
    width(width),
    height(height),
    title(title),
    xLabel(xLabel),
    yLabel(yLabel)
{}

GnuplotDescription::GnuplotDescription(const GnuplotDescription& copy) :
    width{copy.width},
    height{copy.height},
    title{copy.title},
    xLabel{copy.xLabel},
    yLabel{copy.yLabel}
{}

GnuplotDescription& GnuplotDescription::operator = (const GnuplotDescription& copy) {
    this->width = copy.width;
    this->height = copy.height;
    this->title = copy.title;
    this->xLabel = copy.xLabel;
    this->yLabel = copy.yLabel;
    return *this;
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

} } // mw::helpers