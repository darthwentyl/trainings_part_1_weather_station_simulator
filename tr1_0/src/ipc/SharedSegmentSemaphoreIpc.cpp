#include <ipc/SharedSegmentSemaphoreIpc.h>
#include <logger/Log.h>

#include <exceptions/ftok_error.h>
#include <exceptions/sem_error.h>

#include <filesystem>
#include <fstream>
#include <stdexcept>

#include <sys/ipc.h>
#include <sys/sem.h>
#include <unistd.h>

namespace {
    constexpr int NOT_INITIALIZED = -1;
    constexpr int FAILURE = -1;
    constexpr int PERMISSIONS = 0666;
} // anonymous

namespace mw { namespace ipc {

using namespace exceptions;
namespace fs = std::filesystem;

SharedSegmentSemaphoreIpc::SharedSegmentSemaphoreIpc(const std::string& name, const EUsageShmSegment usage)
    : usage{usage}
    , semName{name}
    , semId{NOT_INITIALIZED}
{
    DEBUG("semName: " << semName << " usage: " << static_cast<int>(usage));
}

SharedSegmentSemaphoreIpc::~SharedSegmentSemaphoreIpc() {
    try {
        close();
    } catch(const std::exception& e) {
        ERROR(e.what());
    }
}

void SharedSegmentSemaphoreIpc::open(const std::size_t initial_val) {
    DEBUG("semName: " << semName << " usage: " << static_cast<int>(usage) << " initial_val: " << initial_val);

    if (usage == EUsageShmSegment::UNKNOWN) {
        throw sem_error{__FUNCTION__, __LINE__, "Cannot open semaphore when usage is UNKNOWN"};
    }

    if (!fs::exists(semName)) {
        std::ofstream ofs(semName);
        if (!ofs) {
            throw std::runtime_error{"Cannot create key file."};
        }
    }

    key_t key = ftok(semName.c_str(), semName.length());
    if (key == FAILURE) {
        throw ftok_error{__FUNCTION__, __LINE__, semName, errno};
    }

    if (usage == EUsageShmSegment::CREATOR) {
        if (initial_val == std::numeric_limits<std::size_t>::max()) {
            throw sem_error{__FUNCTION__, __LINE__, "initial_val cannot be not initialized when you are CREATOR"};
        }
        semId = semget(key, 1, IPC_CREAT | IPC_EXCL | PERMISSIONS);
        if (semId == FAILURE) {
            throw sem_error{__FUNCTION__, __LINE__, "semget", errno};
        }
        if (semctl(semId, 0, SETVAL, initial_val) == FAILURE) {
            throw sem_error{__FUNCTION__, __LINE__, "semctl", errno};
        }
    } else {
        semId = semget(key, 1, PERMISSIONS);
        if (semId == FAILURE) {
            throw sem_error{__FUNCTION__, __LINE__, "semget", errno};
        }
    }
    DEBUG("semId: " << semId << " opened");
}

void SharedSegmentSemaphoreIpc::wait() {
    DEBUG("enter pid: " << getpid());
    if (usage == EUsageShmSegment::CREATOR) {
        throw sem_error{__FUNCTION__, __LINE__, "cannot do operation on semaphore if you CREATOR"};
    }

    struct sembuf sb;
    sb.sem_num = 0;        /* Operate on semaphore 0 */
    sb.sem_op = -1;         /* Wait for value to equal 0 */
    sb.sem_flg = 0;

    if (semop(semId, &sb, 1) == FAILURE) {
        ERROR("pid: " << getpid() << " semop(semId: " << semId << ") failed: " << strerror(errno));
        throw sem_error{__FUNCTION__, __LINE__, "semop", errno};
    }
    DEBUG("exit pid: " << getpid());
}

void SharedSegmentSemaphoreIpc::post() {
    DEBUG("enter pid: " << getpid());
    if (usage == EUsageShmSegment::CREATOR) {
        throw sem_error{__FUNCTION__, __LINE__, "cannot do operation on semaphore if you CREATOR"};
    }

    struct sembuf sb;
    sb.sem_num = 0;
    sb.sem_op = 1;
    sb.sem_flg = 0;

    if (semop(semId, &sb, 1) == FAILURE) {
        ERROR("pid: " << getpid() << " semop(semId: " << semId << ") failed: " << strerror(errno));
        throw sem_error{__FUNCTION__, __LINE__, "semop", errno};
    }
    DEBUG("enter pid: " << getpid());
}

void SharedSegmentSemaphoreIpc::close() {
    DEBUG("pid: " << getpid() << " usage: " << static_cast<int>(usage) << " semId: " << semId);
    if (usage == EUsageShmSegment::UNKNOWN || semId == NOT_INITIALIZED) {
        INFO("Semaphore has already closed");
    }
    else if (usage == EUsageShmSegment::CREATOR) {
        if (semctl(semId, 0, IPC_RMID) == FAILURE) {
            setDefault();
            throw sem_error{__FUNCTION__, __LINE__, "semctl", errno};
        }
        if (fs::exists(semName)) {
            INFO("fs::remove(" << semName << "): " << std::boolalpha << fs::remove(semName));
        }
    }
    setDefault();
}

void SharedSegmentSemaphoreIpc::setDefault() {
    DEBUG("");
    usage = EUsageShmSegment::UNKNOWN;
    semName.clear();
    semId = NOT_INITIALIZED;
}

} } // mw::ipc