#include <gtest/gtest.h>

#include <helpers/GnuplotCommandExecutor.h>
#include <helpers/GnuplotCommander.h>
#include <helpers/GnuplotDescription.h>
#include <helpers/GnuplotDescriptionBuilder.h>

#include <mocks/ipc/IpcMock.h>

#include <filesystem>
#include <fstream>

namespace {

using namespace testing;
using namespace mw::helpers;
using namespace mw::mocks;

namespace fs = std::filesystem;

constexpr const std::size_t WIDTH = 600;
constexpr const std::size_t HEIGHT = 400;
constexpr const char* TITLE = "test title";
constexpr const char* X_LABEL = "test x label";
constexpr const char* Y_LABEL = "test y label";
constexpr const char* LEGEND = "test legend";
constexpr const char* DATA_FILE = "test data file";

class GnuplotCommandExecutor_tests : public Test {
public:
    ~GnuplotCommandExecutor_tests() = default;

protected:
    void SetUp() override {
        description = GnuplotDescriptionBuilder()
            .width(WIDTH)
            .height(HEIGHT)
            .title(TITLE)
            .xLabel(X_LABEL)
            .yLabel(Y_LABEL)
            .grid(true)
            .legend(LEGEND)
            .dataFile(DATA_FILE)
            .build();
    }

    void TearDown() override {
        if (fs::exists(DATA_FILE)) {
            fs::remove(DATA_FILE);
        }
    }

    StrictMock<IpcMock> ipcMock;
    GnuplotDescription description;
};

TEST_F(GnuplotCommandExecutor_tests, executeTerminal) {
    auto executor = GnuplotCommandExecutor{ipcMock, description};
    const std::string cmd  = GnuplotCommander::terminal(description.getWidth(), description.getHeight());

    EXPECT_CALL(ipcMock, write(StrEq(cmd))).WillOnce(Return(true));

    executor.execute(ECommand::TERMINAL);
}

TEST_F(GnuplotCommandExecutor_tests, executeTitle) {
    auto executor = GnuplotCommandExecutor{ipcMock, description};
    const std::string cmd = GnuplotCommander::title(description.getTitle());

    EXPECT_CALL(ipcMock, write(StrEq(cmd))).WillOnce(Return(true));

    executor.execute(ECommand::TITLE);
}

TEST_F(GnuplotCommandExecutor_tests, executeTitle_empty) {
    description.setTitle(std::string{});
    auto executor = GnuplotCommandExecutor{ipcMock, description};

    EXPECT_CALL(ipcMock, write(_)).Times(0);

    executor.execute(ECommand::TITLE);
}

TEST_F(GnuplotCommandExecutor_tests, executeAxisLabels) {
    auto executor = GnuplotCommandExecutor{ipcMock, description};
    const std::string cmdXLabel =
        GnuplotCommander::axisLabel(EGnuplotAxis::OX, description.getAxisLabel(EGnuplotAxis::OX));
    const std::string cmdYLabel =
        GnuplotCommander::axisLabel(EGnuplotAxis::OY, description.getAxisLabel(EGnuplotAxis::OY));

    EXPECT_CALL(ipcMock, write(StrEq(cmdXLabel))).WillOnce(Return(true));
    EXPECT_CALL(ipcMock, write(StrEq(cmdYLabel))).WillOnce(Return(true));

    executor.execute(ECommand::AXIS_LABELS);
}

TEST_F(GnuplotCommandExecutor_tests, executeAxisLabels_x_label_empty) {
    description.setAxisLabel(EGnuplotAxis::OX, std::string{});
    auto executor = GnuplotCommandExecutor{ipcMock, description};

    const std::string cmdYLabel =
        GnuplotCommander::axisLabel(EGnuplotAxis::OY, description.getAxisLabel(EGnuplotAxis::OY));

    EXPECT_CALL(ipcMock, write(_)).Times(0);
    EXPECT_CALL(ipcMock, write(StrEq(cmdYLabel))).WillOnce(Return(true));

    executor.execute(ECommand::AXIS_LABELS);
}

TEST_F(GnuplotCommandExecutor_tests, executeAxisLabels_y_label_empty) {
    description.setAxisLabel(EGnuplotAxis::OY, std::string{});
    auto executor = GnuplotCommandExecutor{ipcMock, description};

    const std::string cmdXLabel =
        GnuplotCommander::axisLabel(EGnuplotAxis::OX, description.getAxisLabel(EGnuplotAxis::OX));

    EXPECT_CALL(ipcMock, write(_)).Times(0);
    EXPECT_CALL(ipcMock, write(StrEq(cmdXLabel))).WillOnce(Return(true));

    executor.execute(ECommand::AXIS_LABELS);
}

TEST_F(GnuplotCommandExecutor_tests, executeGrid) {
    auto executor = GnuplotCommandExecutor{ipcMock, description};
    const std::string cmd = GnuplotCommander::grid(description.getGrid());

    EXPECT_CALL(ipcMock, write(StrEq(cmd))).WillOnce(Return(true));

    executor.execute(ECommand::GRID);
}

TEST_F(GnuplotCommandExecutor_tests, executePlotPoints) {
    auto executor = GnuplotCommandExecutor{ipcMock, description};
    const std::string cmd =
        GnuplotCommander::plotPoints(description.getDataFile(), description.getLegend());
    {
        std::ofstream dataFile(DATA_FILE);
    }

    EXPECT_CALL(ipcMock, write(StrEq(cmd))).WillOnce(Return(true));

    executor.execute(ECommand::PLOT_POINTS);
}

TEST_F(GnuplotCommandExecutor_tests, executePlotPoints_dataFile_empty) {
    description.setDataFile(std::string{});
    auto executor = GnuplotCommandExecutor{ipcMock, description};

    EXPECT_CALL(ipcMock, write(_)).Times(0);

    executor.execute(ECommand::PLOT_POINTS);
}

TEST_F(GnuplotCommandExecutor_tests, executePlotPoints_dataFile_not_exists) {
    description.setDataFile(std::string{DATA_FILE} + std::string{".not_exists"});
    auto executor = GnuplotCommandExecutor{ipcMock, description};

    EXPECT_CALL(ipcMock, write(_)).Times(0);

    executor.execute(ECommand::PLOT_POINTS);
}

} // anonymous