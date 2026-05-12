#pragma once

#include <string>

#include <helpers/EGnuplotAxis.h>

namespace mw { namespace helpers {

class GnuplotCommander {
public:
    static std::string terminal(const std::size_t width, const std::size_t height);
    static std::string title(const std::string& title);
    static std::string axisLabel(const EGnuplotAxis axis, const std::string& label);
    static std::string grid(const bool isShow);
    static std::string plotPoints(const std::string& dataFile, const std::string& legend = std::string{});
};

} } // mw::helpers