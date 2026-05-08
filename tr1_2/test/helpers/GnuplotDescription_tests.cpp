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

class GnuplotDescription_tests : public Test {
public:
    ~GnuplotDescription_tests() = default;

protected:
    GnuplotDescription_tests() :
        gnuplotDescription {
            WIDTH,
            HEIGHT,
            TITLE,
            X_LABEL,
            Y_LABEL
        }
    {}

    GnuplotDescription gnuplotDescription;
};

TEST_F(GnuplotDescription_tests, getWidth) {
    EXPECT_EQ(gnuplotDescription.getWidth(), WIDTH);
}

TEST_F(GnuplotDescription_tests, getHeight) {
    EXPECT_EQ(gnuplotDescription.getHeight(), HEIGHT);
}

TEST_F(GnuplotDescription_tests, getTitle) {
    EXPECT_STREQ(gnuplotDescription.getTitle().c_str(), TITLE);
}

TEST_F(GnuplotDescription_tests, getAxisLabel_x_label) {
    EXPECT_STREQ(gnuplotDescription.getAxisLabel(EGnuplotAxis::OX).c_str(), X_LABEL);
}

TEST_F(GnuplotDescription_tests, getAxisLabel_y_label) {
    EXPECT_STREQ(gnuplotDescription.getAxisLabel(EGnuplotAxis::OY).c_str(), Y_LABEL);
}

TEST_F(GnuplotDescription_tests, copy_constructor) {
    GnuplotDescription newGnuplotDescription{gnuplotDescription};
    EXPECT_EQ(newGnuplotDescription.getWidth(), gnuplotDescription.getWidth());
    EXPECT_EQ(newGnuplotDescription.getHeight(), gnuplotDescription.getHeight());
    EXPECT_STREQ(
        newGnuplotDescription.getTitle().c_str(),
        gnuplotDescription.getTitle().c_str());
    EXPECT_STREQ(
        newGnuplotDescription.getAxisLabel(EGnuplotAxis::OX).c_str(), gnuplotDescription.getAxisLabel(EGnuplotAxis::OX).c_str());
    EXPECT_STREQ(
        newGnuplotDescription.getAxisLabel(EGnuplotAxis::OY).c_str(),
        gnuplotDescription.getAxisLabel(EGnuplotAxis::OY).c_str());
}

TEST_F(GnuplotDescription_tests, operator_assignment) {
    GnuplotDescription newGnuplotDescription{1, 2, "title", "xlabel", "ylabel"};
    newGnuplotDescription = gnuplotDescription;
    EXPECT_EQ(newGnuplotDescription.getWidth(), gnuplotDescription.getWidth());
    EXPECT_EQ(newGnuplotDescription.getHeight(), gnuplotDescription.getHeight());
    EXPECT_STREQ(
        newGnuplotDescription.getTitle().c_str(),
        gnuplotDescription.getTitle().c_str());
    EXPECT_STREQ(
        newGnuplotDescription.getAxisLabel(EGnuplotAxis::OX).c_str(),
        gnuplotDescription.getAxisLabel(EGnuplotAxis::OX).c_str());
    EXPECT_STREQ(
        newGnuplotDescription.getAxisLabel(EGnuplotAxis::OY).c_str(),
        gnuplotDescription.getAxisLabel(EGnuplotAxis::OY).c_str());
}

} // anonymous