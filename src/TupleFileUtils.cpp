#include "TupleFileUtils.h"

int linda::TupleFileUtils::lockRecord(int fd, int length, int record_id) {
    struct flock lck;
    lck.l_type = F_WRLCK;
    lck.l_whence = 0;
    lck.l_start = record_id * length;
    lck.l_len = length;
    lck.l_pid = getpid();

    return fcntl(fd, F_SETLKW, &lck);
}

int linda::TupleFileUtils::unlockRecord(int fd, int length, int record_id) {
    struct flock lck;
    lck.l_type = F_UNLCK;
    lck.l_whence = 0;
    lck.l_start = record_id * length;
    lck.l_len = length;
    lck.l_pid = getpid();

    return fcntl(fd, F_SETLKW, &lck);
}

int linda::TupleFileUtils::readRecord(int fd, struct tuple *tuple_ptr, int record_id) {
    lseek(fd, record_id * (sizeof(struct tuple)), 0);
    return read(fd, tuple_ptr, sizeof(struct tuple));
}

int linda::TupleFileUtils::writeRecord(int fd, struct tuple *tuple_ptr, int record_id) {
    lseek(fd, record_id * (sizeof(struct tuple)), 0);
    return write(fd, tuple_ptr, sizeof(struct tuple));
}

int linda::TupleFileUtils::checkRecordTaken(int fd, int record_id) {
    char flag;
    lseek(fd, sizeof(char), 0);
    if (!read(fd, &flag, sizeof(char))) {
        return flag;
    }
    else {
        return -1;
    }
}









