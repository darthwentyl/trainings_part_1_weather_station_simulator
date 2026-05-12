#pragma once

namespace mw { namespace ipc {

enum class EUsageShmSegment {
    UNKNOWN = -1,
    CREATOR = 0,
    CLIENT = 1
};

} } // mw::ipc