#include <helpers/GnuplotCommandExecutor.h>
#include <helpers/GnuplotCommander.h>
#include <ipc/IIpc.h>
#include <logger/Log.h>

#include <filesystem>

namespace mw { namespace helpers {

using namespace mw::ipc;
namespace fs = std::filesystem;

GnuplotCommandExecutor::GnuplotCommandExecutor(IIpc& ipc, const GnuplotDescription& description) :
    ipc{ipc},
    description{description}
{}

void GnuplotCommandExecutor::execute(const EGnuplotCommand cmd) const {
    switch (cmd) {
        case EGnuplotCommand::TERMINAL: executeTerminal(); break;
        case EGnuplotCommand::TITLE: executeTitle(); break;
        case EGnuplotCommand::AXIS_LABELS: executeAxisLabels(); break;
        case EGnuplotCommand::GRID: executeGrid(); break;
        case EGnuplotCommand::PLOT_POINTS: executePlotPoints(); break;
    }
}

void GnuplotCommandExecutor::executeTerminal() const {
    ipc.writeData(GnuplotCommander::terminal(description.getWidth(), description.getHeight()));
}

void GnuplotCommandExecutor::executeTitle() const {
    if (description.getTitle().empty()) {
        INFO("Title is empty. No execute");
        return;
    }
    ipc.writeData(GnuplotCommander::title(description.getTitle()));
}

void GnuplotCommandExecutor::executeAxisLabels() const {
    std::string label = description.getAxisLabel(EGnuplotAxis::OX);
    if (!label.empty()) {
        ipc.writeData(GnuplotCommander::axisLabel(EGnuplotAxis::OX, label));
    }
    label = description.getAxisLabel(EGnuplotAxis::OY);
    if (!label.empty()) {
        ipc.writeData(GnuplotCommander::axisLabel(EGnuplotAxis::OY, label));
    }
}

void GnuplotCommandExecutor::executeGrid() const {
    ipc.writeData(GnuplotCommander::grid(description.getGrid()));
}

void GnuplotCommandExecutor::executePlotPoints() const {
    const std::string dataFile = description.getDataFile();
    if (dataFile.empty() || !fs::exists(dataFile)) {
        INFO("No data to plot! File: " << dataFile);
        return;
    }
    ipc.writeData(GnuplotCommander::plotPoints(dataFile, description.getLegend()));
}

} } // mw::helpers