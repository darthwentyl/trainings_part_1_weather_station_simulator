#pragma once

namespace mw { namespace proc_managers {

class IProcManager {
public:
    virtual ~IProcManager() = default;

    virtual void loop() = 0;
};

} } // mw::proc_managers