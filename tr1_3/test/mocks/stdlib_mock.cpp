#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <dlfcn.h>
#include <semaphore.h>
#include <sys/stat.h>
#include <sys/shm.h>
#include <sys/socket.h>

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

FILE* popen(const char* command, const char* type) {
    return StdLibStaticMock::get().popen(command, type);
}

int pclose(FILE* stream) {
    return StdLibStaticMock::get().pclose(stream);
}

char* fgets(char s[], int size, FILE* stream) {
    return StdLibStaticMock::get().fgets(s, size, stream);
}

int fputs(const char* s, FILE* stream) {
    return StdLibStaticMock::get().fputs(s, stream);
}

int fflush([[maybe_unused]] FILE* stream) {
    // TODO: I tried to mock but to many places are called this function
    // so I decided always say 0 - success
    return 0;
}

int socket(int domain, int type, int protocol) {
    return StdLibStaticMock::get().socket(domain, type, protocol);
}

int setsockopt(int sockfd, int level, int optname, const void* optval, socklen_t optlen) {
    return StdLibStaticMock::get().setsockopt(sockfd, level, optname, optval, optlen);
}

int bind(int sockfd, const struct sockaddr* addr, socklen_t addrlen) {
    return StdLibStaticMock::get().bind(sockfd, addr, addrlen);
}

int listen(int sockfd, int backlog) {
    return StdLibStaticMock::get().listen(sockfd, backlog);
}

int accept(int sockfd, struct sockaddr* addr, socklen_t* addrlen) {
    return StdLibStaticMock::get().accept(sockfd, addr, addrlen);
}

int close(int fd) {
    return StdLibStaticMock::get().close(fd);
}

ssize_t read(int fd, void* buf, size_t count) {
    return StdLibStaticMock::get().read(fd, buf, count);
}

ssize_t write(int fd, const void* buf, size_t count) {
    return StdLibStaticMock::get().write(fd, buf, count);
}


} // extern "C"