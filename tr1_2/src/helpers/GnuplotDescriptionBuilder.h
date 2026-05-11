#pragma once

#include <helpers/GnuplotDescription.h>

namespace mw { namespace helpers {

class GnuplotDescriptionBuilder {
public:
    GnuplotDescriptionBuilder& width(const std::size_t width);
    GnuplotDescriptionBuilder& height(const std::size_t height);
    GnuplotDescriptionBuilder& title(const std::string& title);
    GnuplotDescriptionBuilder& xLabel(const std::string& xLabel);
    GnuplotDescriptionBuilder& yLabel(const std::string& yLabel);
    GnuplotDescriptionBuilder& legend(const std::string& legend);
    GnuplotDescriptionBuilder& dataFile(const std::string& dataFile);
    GnuplotDescriptionBuilder& grid(const bool grid);

    GnuplotDescription build();
private:
    GnuplotDescription gnuplotDescription;
};

} } // mw::helpers