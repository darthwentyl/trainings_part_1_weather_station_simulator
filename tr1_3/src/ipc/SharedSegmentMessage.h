#pragma once

#include <sstream>

namespace mw { namespace ipc {

class SharedSegmentMessage {
public:
    SharedSegmentMessage();

    std::string serialize();
    void deserialize(const char* mem);

private:
    std::string data;
    bool isExit;
};

} } // mw::ipc