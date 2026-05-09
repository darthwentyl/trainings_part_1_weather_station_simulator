#include <gtest/gtest.h>

#include <helpers/GnuplotDescription.h>

namespace {

using namespace testing;
using namespace mw::helpers;

constexpr const std::size_t WIDTH = 800;
constexpr const std::size_t HEIGHT = 600;
constexpr const char* TITLE = "test_title";
constexpr const char* X_LABEL = "test xlabel";
constexpr const char* Y_LABEL = "test ylabel";
constexpr const char* LEGEND = "test legend";
constexpr const char* DATA_FILE = "test data file";

class GnuplotDescription_tests : public Test {
public:
    ~GnuplotDescription_tests() = default;

protected:
    GnuplotDescription gnuplotDescription;
};

TEST_F(GnuplotDescription_tests, set_getWidth) {
    gnuplotDescription.setWidth(WIDTH);
    EXPECT_EQ(gnuplotDescription.getWidth(), WIDTH);
}

TEST_F(GnuplotDescription_tests, set_getHeight) {
    gnuplotDescription.setHeight(HEIGHT);
    EXPECT_EQ(gnuplotDescription.getHeight(), HEIGHT);
}

TEST_F(GnuplotDescription_tests, set_getTitle) {
    gnuplotDescription.setTitle(TITLE);
    EXPECT_STREQ(gnuplotDescription.getTitle().c_str(), TITLE);
}

TEST_F(GnuplotDescription_tests, set_getAxisLabel_x_label) {
    gnuplotDescription.setAxisLabel(EGnuplotAxis::OX, X_LABEL);
    EXPECT_STREQ(gnuplotDescription.getAxisLabel(EGnuplotAxis::OX).c_str(), X_LABEL);
}

TEST_F(GnuplotDescription_tests, set_getAxisLabel_y_label) {
    gnuplotDescription.setAxisLabel(EGnuplotAxis::OY, Y_LABEL);
    EXPECT_STREQ(gnuplotDescription.getAxisLabel(EGnuplotAxis::OY).c_str(), Y_LABEL);
}

TEST_F(GnuplotDescription_tests, set_getLegend) {
    gnuplotDescription.setLegend(LEGEND);
    EXPECT_STREQ(gnuplotDescription.getLegend().c_str(), LEGEND);
}

TEST_F(GnuplotDescription_tests, set_getDataFile) {
    gnuplotDescription.setDataFile(DATA_FILE);
    EXPECT_STREQ(gnuplotDescription.getDataFile().c_str(), DATA_FILE);
}

} // anonymous