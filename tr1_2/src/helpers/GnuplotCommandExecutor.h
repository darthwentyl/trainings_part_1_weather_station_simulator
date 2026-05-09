#pragma once

#include <helpers/GnuplotDescription.h>
#include <helpers/EGnuplotCommand.h>

namespace mw { namespace ipc {

    class IIpc;

} } // mw::ipc

namespace mw { namespace helpers {

class GnuplotCommandExecutor {
public:
    GnuplotCommandExecutor(ipc::IIpc& ipc, const GnuplotDescription& description);

    void execute(const ECommand cmd) const;

private:
    void execute_title() const;

    ipc::IIpc& ipc;
    GnuplotDescription description;
};

} } // mw::helpers