#include <helpers/GnuplotCommandExecutor.h>
#include <helpers/GnuplotCommander.h>
#include <ipc/IIpc.h>
#include <logger/Log.h>

namespace mw { namespace helpers {

using namespace mw::ipc;

GnuplotCommandExecutor::GnuplotCommandExecutor(IIpc& ipc, const GnuplotDescription& description) :
    ipc{ipc},
    description{description}
{}

void GnuplotCommandExecutor::execute(const ECommand cmd) const {
    switch (cmd) {
        case ECommand::TERMINAL: execute_terminal(); break;
        case ECommand::TITLE: execute_title(); break;
    }
}

void GnuplotCommandExecutor::execute_terminal() const {
    ipc.write(GnuplotCommander::terminal(description.getWidth(), description.getHeight()));
}

void GnuplotCommandExecutor::execute_title() const {
    if (description.getTitle().empty()) {
        INFO("Title is empty. No execute");
        return;
    }
    ipc.write(GnuplotCommander::title(description.getTitle()));
}

} } // mw::helpers