#include <ipc/SharedSegmentMessage.h>
#include <iostream>
namespace mw { namespace ipc {

SharedSegmentMessage::SharedSegmentMessage() : data{}, isExit{false} {}

std::string SharedSegmentMessage::serialize() {
    std::ostringstream oss;
    if (isExit) {
        return "exit";
    }
    oss << data << "isExit: " << isExit;
    return oss.str();
}

void SharedSegmentMessage::deserialize(const char* mem) {
    if (std::string{mem} == "exit") {
        isExit = true;
        return;
    }
    std::istringstream iss{mem};
    std::string buff;
    data.clear();
    while(iss >> buff) {
        if (buff == "exit") {
            isExit = true;
        }
        else if (buff == "isExit:") {
            iss >> isExit;
        } else {
            data.append(buff + " ");
        }
    }
}

} } // mw::ipc