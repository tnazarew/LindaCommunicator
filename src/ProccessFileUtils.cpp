#include "ProccessFileUtils.h"

int linda::ProccessFileUtils::lockRecord(int fd, int length, int record_id) {
    struct flock lck;
    lck.l_type = F_WRLCK;
    lck.l_whence = 0;
    lck.l_start = record_id * length;
    lck.l_len = length;
    lck.l_pid = getpid();

    return fcntl(fd, F_SETLKW, &lck);
}

int linda::ProccessFileUtils::unlockRecord(int fd, int length, int record_id) {
    struct flock lck;
    lck.l_type = F_UNLCK;
    lck.l_whence = 0;
    lck.l_start = record_id * length;
    lck.l_len = length;
    lck.l_pid = getpid();

    return fcntl(fd, F_SETLKW, &lck);
}

int linda::ProccessFileUtils::readRecord(int fd, struct process *process_ptr, int record_id) {
    lseek(fd, record_id * (sizeof(struct process)), 0);
    return read(fd, process_ptr, sizeof(struct process));
}

int linda::ProccessFileUtils::writeRecord(int fd, struct process *process_ptr, int record_id) {
    lseek(fd, record_id * (sizeof(struct process)), 0);
    return write(fd, process_ptr, sizeof(struct process));
}

int linda::ProccessFileUtils::checkRecordTaken(int fd, int record_id) {
    char flag;
    lseek(fd, record_id * sizeof(struct process), 0);
    if (!read(fd, &flag, sizeof(char))) {
        return flag;
    }
    else {
        return -1;
    }
}

int linda::ProccessFileUtils::setRecordTaken(int fd, int record_id, char taken) {
    lseek(fd, record_id * sizeof(struct process), 0);
    return write(fd, &taken, sizeof(char));
}









