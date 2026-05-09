#include <gtest/gtest.h>

#include <helpers/GnuplotCommandExecutor.h>
#include <helpers/GnuplotCommander.h>
#include <helpers/GnuplotDescription.h>
#include <helpers/GnuplotDescriptionBuilder.h>

#include <mocks/ipc/IpcMock.h>

namespace {

using namespace testing;
using namespace mw::helpers;
using namespace mw::mocks;

constexpr const char* TITLE = "test title";

class GnuplotCommandExecutor_tests : public Test {
public:
    ~GnuplotCommandExecutor_tests() = default;

protected:
    void SetUp() override {
        description = GnuplotDescriptionBuilder()
            .title(TITLE)
            .build();
    }

    StrictMock<IpcMock> ipcMock;
    GnuplotDescription description;
};

TEST_F(GnuplotCommandExecutor_tests, execut_title) {
    auto executor = GnuplotCommandExecutor{ipcMock, description};
    const std::string cmd = GnuplotCommander::title(description.getTitle());

    EXPECT_CALL(ipcMock, write(cmd)).WillOnce(Return(true));

    executor.execute(ECommand::TITLE);
}

TEST_F(GnuplotCommandExecutor_tests, execut_title_empty) {
    description.setTitle(std::string{});
    auto executor = GnuplotCommandExecutor{ipcMock, description};

    EXPECT_CALL(ipcMock, write(_)).Times(0);

    executor.execute(ECommand::TITLE);
}

} // anonymous