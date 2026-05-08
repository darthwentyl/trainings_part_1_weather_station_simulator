#pragma once

#include <string>

#include <helpers/EGnuplotAxis.h>

namespace mw { namespace helpers {

class GnuplotDescription {
public:
    GnuplotDescription(const std::size_t width,
        const std::size_t height,
        const std::string& title,
        const std::string& xLabel,
        const std::string& yLabel);
    GnuplotDescription(const GnuplotDescription& copy);
    GnuplotDescription& operator = (const GnuplotDescription& copy);

    const std::size_t& getWidth() const;
    const std::size_t& getHeight() const;
    const std::string& getTitle() const;
    const std::string& getAxisLabel(const EGnuplotAxis axis) const;

private:
    std::size_t width;
    std::size_t height;
    std::string title;
    std::string xLabel;
    std::string yLabel;
};

} } // mw::helpers