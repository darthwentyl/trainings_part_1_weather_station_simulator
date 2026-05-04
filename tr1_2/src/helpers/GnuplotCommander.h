#pragma once

#include <string>

namespace mw { namespace helpers {

enum class EGnuplotAxis {
    OX = 0,
    OY = 1
};

class GnuplotCommander {
public:
    static std::string terminal(const std::size_t width, const std::size_t height);
    static std::string title(const std::string& title);
    static std::string axisLabel(const EGnuplotAxis axis, const std::string& label);
    static std::string grid(const bool isShow);
    static std::string plotPoints(const std::string& dataFile, const std::string& legend = std::string{});
};

} } // mw::helpers