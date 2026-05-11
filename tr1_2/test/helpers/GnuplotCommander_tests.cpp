#include <gtest/gtest.h>

#include <helpers/GnuplotCommander.h>

namespace {

using namespace testing;
using namespace mw::helpers;

class GnuplotCommander_tests : public Test {
public:
    ~GnuplotCommander_tests() = default;
};

TEST_F(GnuplotCommander_tests, terminal) {
    const char* cmd = "set terminal qt size 800,600\n";
    EXPECT_STREQ(GnuplotCommander::terminal(800, 600).c_str(), cmd);
}

TEST_F(GnuplotCommander_tests, terminal_width_0) {
    const char* cmd = "set terminal qt\n";
    EXPECT_STREQ(GnuplotCommander::terminal(0, 1).c_str(), cmd);
}

TEST_F(GnuplotCommander_tests, terminal_height_0) {
    const char* cmd  = "set terminal qt\n";
    EXPECT_STREQ(GnuplotCommander::terminal(1, 0).c_str(), cmd);
}

TEST_F(GnuplotCommander_tests, title) {
    const char* cmd = "set title 'my test title'\n";
    EXPECT_STREQ(GnuplotCommander::title("my test title").c_str(), cmd);
}

TEST_F(GnuplotCommander_tests, xlabel) {
    const char* cmd = "set xlabel 'my test xlabel'\n";
    EXPECT_STREQ(GnuplotCommander::axisLabel(EGnuplotAxis::OX, "my test xlabel").c_str(), cmd);
}

TEST_F(GnuplotCommander_tests, ylabel) {
    const char* cmd = "set ylabel 'my test ylabel'\n";
    EXPECT_STREQ(GnuplotCommander::axisLabel(EGnuplotAxis::OY, "my test ylabel").c_str(), cmd);
}

TEST_F(GnuplotCommander_tests, grid_on) {
    const char* cmd = "set grid\n";
    EXPECT_STREQ(GnuplotCommander::grid(true).c_str(), cmd);
}

TEST_F(GnuplotCommander_tests, grid_off) {
    const char* cmd = "unset grid\n";
    EXPECT_STREQ(GnuplotCommander::grid(false).c_str(), cmd);
}

TEST_F(GnuplotCommander_tests, plotPoints_with_legend) {
    const char* cmd = "plot \"data.dat\" title \"test_legend\" with linespoints\n";
    EXPECT_STREQ(GnuplotCommander::plotPoints("data.dat", "test_legend").c_str(), cmd);
}

TEST_F(GnuplotCommander_tests, plotPoints_without_legend) {
    const char* cmd = "plot \"data.dat\" notitle with linespoints\n";
    EXPECT_STREQ(GnuplotCommander::plotPoints("data.dat").c_str(), cmd);
}

} // anonymous