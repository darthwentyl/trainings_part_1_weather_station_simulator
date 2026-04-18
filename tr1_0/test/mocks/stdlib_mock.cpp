#include <fcntl.h>
#include <sys/stat.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <dlfcn.h>
#include <semaphore.h>
#include <sys/shm.h>

#include <mocks/StdLibStaticMock.h>

using namespace mw::mocks;

extern "C" {

key_t ftok(const char *pathname, int proj_id) {
    return StdLibStaticMock::get().ftok(pathname, proj_id);
}

int semget(key_t key, int nsems, int semflg) {
    return StdLibStaticMock::get().semget(key, nsems, semflg);
}

int semctl(int semid, int semnum, int op) {
    return StdLibStaticMock::get().semctl(semid, semnum, op);
}

int semop(int semid, struct sembuf *sops, size_t nsops) {
    return StdLibStaticMock::get().semop(semid, sops, nsops);
}

pid_t getpid() {
    return StdLibStaticMock::get().getpid();
}

int shmget(key_t key, size_t size, int shmflg) {
    return StdLibStaticMock::get().shmget(key, size, shmflg);
}

void* shmat(int shmid, const void* shmaddr, int shmflg) {
    return StdLibStaticMock::get().shmat(shmid, shmaddr, shmflg);
}

int shmctl(int shmid, int op, struct shmid_ds *buf) {
    return StdLibStaticMock::get().shmctl(shmid, op, buf);
}

int shmdt(const void* shmaddr) {
    return StdLibStaticMock::get().shmdt(shmaddr);
}

} // extern "C"