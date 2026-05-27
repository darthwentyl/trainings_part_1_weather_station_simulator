#include <proc_managers/workers/UserCmdWorker.h>

#include <logger/Log.h>
#include <ipc/IIpc.h>

namespace mw { namespace proc_managers { namespace workers {

using namespace mw::ipc;

UserCmdWorker::UserCmdWorker(IIpc& ipcMemory, IIpc& ipcSocket, const std::size_t bufferSize) :
    Worker{ipcMemory},
    userCmdListener{ipcSocket, bufferSize}
{}

void UserCmdWorker::startWorking() {
    Worker::startWorking();

    if (userCmdListener.isListening()) {
        INFO("User command listener has already started");
        return;
    }
    userCmdListener.startListening();
}

void UserCmdWorker::processData() {
    if (!isWorking()) {
        INFO("Worker has not started yet");
        return;
    }

    std::string msg = ipcMem().read();
    DEBUG("read: " << msg);

    if (msg == "exit") {
        stopWorking();
    } else {
        WeatherData data;
        data.deserialize(msg);
        userCmdListener.addWeatherData(data);
    }
}

void UserCmdWorker::stopWorking() {
    Worker::stopWorking();

    if (!userCmdListener.isListening()) {
        INFO("User command listener has already stopped");
        return;
    }
    userCmdListener.stopListening();
}

} } } // mw::proc_managers::worker