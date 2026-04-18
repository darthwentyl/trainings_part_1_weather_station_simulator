#include <ipc/SharedSegmentMemoryIpc.h>
#include <logger/Log.h>

#include <exceptions/ftok_error.h>
#include <exceptions/shm_error.h>

#include <filesystem>
#include <fstream>
#include <stdexcept>

#include <sys/ipc.h>
#include <sys/shm.h>
#include <unistd.h>

#define SHMAT_FAILURE (void *)-1

namespace {

constexpr int NOT_INITIALIZED = -1;
constexpr int FAILURE = -1;
constexpr int PERMISSIONS = 0666;

} // anonymous

namespace mw { namespace ipc {

using namespace mw::exceptions;
namespace fs = std::filesystem;

SharedSegmentMemoryIpc::SharedSegmentMemoryIpc(const std::string& name, const std::size_t size, const EUsageShmSegment usage)
    : name{name}
    , size{size}
    , usage{usage}
    , shmId{NOT_INITIALIZED}
    , shmMem{nullptr}
{
    DEBUG("name: " << name << " size: " << size << " usage: " << static_cast<int>(usage));
}

SharedSegmentMemoryIpc::~SharedSegmentMemoryIpc() {
    try {
        close();
    } catch(const std::exception& e) {
        ERROR(e.what());
    }
}

void SharedSegmentMemoryIpc::open() {
    DEBUG("");

    if (usage == EUsageShmSegment::UNKNOWN) {
        throw shm_error{__FUNCTION__, __LINE__, "Cannot open semaphore when usage is UNKNOWN"};
    }

    if (size == 0) {
        throw shm_error{__FUNCTION__, __LINE__, "Size cannot be 0"};
    }

    if (!fs::exists(name)) {
        std::ofstream ofs(name);
        if (!ofs) {
            throw std::runtime_error{"Cannot create key file."};
        }
    }

    key_t key = ftok(name.c_str(), name.length());
    if (key == FAILURE) {
        throw ftok_error{__FUNCTION__, __LINE__, name, errno};
    }

    if (usage == EUsageShmSegment::CREATOR) {
        shmId = shmget(key, size, PERMISSIONS | IPC_CREAT | IPC_EXCL);
        if (shmId == FAILURE) {
            throw shm_error{__FUNCTION__, __LINE__, name, size, errno};
        }
    } else {
        shmId = shmget(key, size, PERMISSIONS);
        if (shmId == FAILURE) {
            throw shm_error{__FUNCTION__, __LINE__, name, size, errno};
        }
        shmMem = static_cast<char*>(shmat(shmId, nullptr, 0));
        if (shmMem == SHMAT_FAILURE) {
            throw shm_error{__FUNCTION__, __LINE__, "shmat", errno};
        }
    }

    DEBUG("shmget(size: " << size << "): shmId: " << shmId << " created");
}

void SharedSegmentMemoryIpc::close() {
    DEBUG("usage: " << static_cast<int>(usage) << " shmId: " << shmId);

    if (usage == EUsageShmSegment::UNKNOWN || shmId == NOT_INITIALIZED) {
        INFO("Memory segment has alreay closed");
    } else if (usage == EUsageShmSegment::CREATOR) {
        if (shmctl(shmId, IPC_RMID, nullptr) == FAILURE) {
            setDefault();
            throw shm_error{__FUNCTION__, __LINE__, "shmctl", errno};
        }
        if (fs::exists(name)) {
            INFO("fs::remove(" << name << "): " << std::boolalpha << fs::remove(name));
        }
    } else {
        if (shmdt(shmMem) == FAILURE) {
            setDefault();
            throw shm_error{__FUNCTION__, __LINE__, "shmdt", errno};
        }
    }
    setDefault();
}

std::string SharedSegmentMemoryIpc::read() const {
    DEBUG("pid: " << getpid());

    if (usage == EUsageShmSegment::CREATOR) {
        throw shm_error{__FUNCTION__, __LINE__, "cannot read data if you are CREATOR"};
    }

    if (shmMem == nullptr) {
        throw shm_error{__FUNCTION__, __LINE__, "shmMem is nullptr"};
    }

    INFO("shmMem: " << shmMem);
    return std::string{shmMem};
}

bool SharedSegmentMemoryIpc::write(const std::string& msg) {
    DEBUG("pid: " << getpid() << " msg: " << msg);

    if (usage == EUsageShmSegment::CREATOR) {
        throw shm_error{__FUNCTION__, __LINE__, "cannot write data if you are CREATOR"};
    }

    if (shmMem == nullptr) {
        throw shm_error{__FUNCTION__, __LINE__, "shmMem is nullptr"};
    }

    if (msg.size() > size || msg.size() == 0) {
        ERROR("msg.size() = " << msg.size());
        throw shm_error{__FUNCTION__, __LINE__, "message is too big or equal 0"};
    }

    std::fill(shmMem, shmMem + size, 0);
    std::copy(msg.cbegin(), msg.cend(), shmMem);

    return true;
}

void SharedSegmentMemoryIpc::setDefault() {
    name.clear();
    size = 0;
    usage = EUsageShmSegment::UNKNOWN;
    shmId = NOT_INITIALIZED;
    shmMem = nullptr;
}

} } // mw::ipc