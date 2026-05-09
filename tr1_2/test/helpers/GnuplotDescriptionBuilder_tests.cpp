#include <gtest/gtest.h>

#include <helpers/GnuplotDescriptionBuilder.h>

namespace {

using namespace testing;
using namespace mw::helpers;

// copy from GnuplotDescription.cpp
constexpr const std::size_t MAX_WIDTH = 1000;
constexpr const std::size_t MAX_HEIGHT = 1000;

class GnuplotDescriptionBuilder_tests : public Test {
public:
    ~GnuplotDescriptionBuilder_tests() = default;

protected:
    GnuplotDescriptionBuilder builder;
};

TEST_F(GnuplotDescriptionBuilder_tests, width_correct) {
    const std::size_t testWidth = MAX_WIDTH;
    auto description = builder.width(testWidth).build();
    EXPECT_EQ(description.getWidth(), testWidth);
}

TEST_F(GnuplotDescriptionBuilder_tests, width_too_large) {
    const std::size_t testWidth = MAX_WIDTH + 1;
    auto description = builder.width(testWidth).build();
    EXPECT_EQ(description.getWidth(), 0);
}

TEST_F(GnuplotDescriptionBuilder_tests, height_correct) {
    const std::size_t testHeight = MAX_HEIGHT;
    auto description = builder.height(testHeight).build();
    EXPECT_EQ(description.getHeight(), testHeight);
}

TEST_F(GnuplotDescriptionBuilder_tests, height_too_large) {
    const std::size_t testHeight = MAX_HEIGHT + 1;
    auto description = builder.height(testHeight).build();
    EXPECT_EQ(description.getHeight(), 0);
}

TEST_F(GnuplotDescriptionBuilder_tests, title) {
    const std::string testTitle{"test title"};
    auto description = builder.title(testTitle).build();
    EXPECT_STREQ(description.getTitle().c_str(), testTitle.c_str());
}

TEST_F(GnuplotDescriptionBuilder_tests, x_label) {
    const std::string testXLabel{"test x label"};
    auto description = builder.xLabel(testXLabel).build();
    EXPECT_STREQ(description.getAxisLabel(EGnuplotAxis::OX).c_str(), testXLabel.c_str());
}

TEST_F(GnuplotDescriptionBuilder_tests, y_label) {
    const std::string testYLabel{"test y label"};
    auto description = builder.yLabel(testYLabel).build();
    EXPECT_STREQ(description.getAxisLabel(EGnuplotAxis::OY).c_str(), testYLabel.c_str());
}

TEST_F(GnuplotDescriptionBuilder_tests, legend) {
    const std::string testLegend{"test legend"};
    auto description = builder.legend(testLegend).build();
    EXPECT_STREQ(description.getLegend().c_str(), testLegend.c_str());
}

TEST_F(GnuplotDescriptionBuilder_tests, data_file) {
    const std::string testDataFile{"test data file"};
    auto description = builder.dataFile(testDataFile).build();
    EXPECT_STREQ(description.getDataFile().c_str(), testDataFile.c_str());
}

TEST_F(GnuplotDescriptionBuilder_tests, build_all) {
    const std::size_t testWidth = MAX_WIDTH;
    const std::size_t testHeight = MAX_HEIGHT;
    const std::string testTitle{"test title"};
    const std::string testXLabel{"test x label"};
    const std::string testYLabel{"test y label"};
    const std::string testLegend{"test legend"};
    const std::string testDataFile{"test data file"};

    auto description = builder
        .width(testWidth)
        .height(testHeight)
        .title(testTitle)
        .xLabel(testXLabel)
        .yLabel(testYLabel)
        .legend(testLegend)
        .dataFile(testDataFile)
        .build();

    EXPECT_EQ(description.getWidth(), testWidth);
    EXPECT_EQ(description.getHeight(), testHeight);
    EXPECT_STREQ(description.getTitle().c_str(), testTitle.c_str());
    EXPECT_STREQ(description.getAxisLabel(EGnuplotAxis::OX).c_str(), testXLabel.c_str());
    EXPECT_STREQ(description.getAxisLabel(EGnuplotAxis::OY).c_str(), testYLabel.c_str());
    EXPECT_STREQ(description.getLegend().c_str(), testLegend.c_str());
    EXPECT_STREQ(description.getDataFile().c_str(), testDataFile.c_str());
}

} // anonymous