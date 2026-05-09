#pragma once

#include <string>

#include <helpers/EGnuplotAxis.h>

namespace mw { namespace helpers {

class GnuplotDescription {
public:
    GnuplotDescription();

    void setWidth(const std::size_t width);
    void setHeight(const std::size_t height);
    void setTitle(const std::string& title);
    void setAxisLabel(const EGnuplotAxis axis, const std::string& label);
    void setLegend(const std::string& legend);
    void setDataFile(const std::string& dataFile);

    const std::size_t& getWidth() const;
    const std::size_t& getHeight() const;
    const std::string& getTitle() const;
    const std::string& getAxisLabel(const EGnuplotAxis axis) const;
    const std::string& getLegend() const;
    const std::string& getDataFile() const;

private:
    std::size_t width;
    std::size_t height;
    std::string title;
    std::string xLabel;
    std::string yLabel;
    std::string legend;
    std::string dataFile;
};

} } // mw::helpers