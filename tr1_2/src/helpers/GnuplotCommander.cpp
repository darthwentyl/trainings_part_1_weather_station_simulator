#include <helpers/GnuplotCommander.h>

#include <sstream>

namespace mw { namespace helpers {

namespace {

static const char* axis_label[] {
    /*EGnuplotAxis::OX*/ "xlabel",
    /*EGnuplotAxis::OY*/ "ylabel"
};

} // anonymous

std::string GnuplotCommander::terminal(const std::size_t width, const std::size_t height) {
    std::ostringstream oss;
    oss << "set terminal wxt size " << width << "," << height << "\n";
    return oss.str();
}

std::string GnuplotCommander::title(const std::string& title) {
    std::ostringstream oss;
    oss << "set title '" << title << "'\n";
    return oss.str();
}

std::string GnuplotCommander::axisLabel(const EGnuplotAxis axis, const std::string& label) {
    std::ostringstream oss;
    oss << "set " << axis_label[static_cast<std::size_t>(axis)] << " '" << label << "'\n";
    return oss.str();
}

std::string GnuplotCommander::grid(const bool isShow) {
    std::ostringstream oss;
    if (isShow) {
        oss << "set grid\n";
    } else {
        oss << "unset grid\n";
    }
    return oss.str();
}

std::string GnuplotCommander::plotPoints(const std::string& dataFile, const std::string& legend) {
    std::ostringstream oss;
    if (legend.empty()) {
        oss << "plot \"" << dataFile << "\" notitle with linespoints\n";
    } else {
        oss << "plot \"" << dataFile << "\" title \"" << legend << "\" with linespoints\n";
    }
    return oss.str();
}

} } // mw::helpers