#include "ProcessFileUtils.h"

int linda::ProcessFileUtils::lockRecord(int fd, int length, int record_id) {
    struct flock lck;
    lck.l_type = F_WRLCK;
    lck.l_whence = 0;
    lck.l_start = record_id * length;
    lck.l_len = length;
    lck.l_pid = getpid();

    return fcntl(fd, F_SETLKW, &lck);
}

int linda::ProcessFileUtils::unlockRecord(int fd, int length, int record_id) {
    struct flock lck;
    lck.l_type = F_UNLCK;
    lck.l_whence = 0;
    lck.l_start = record_id * length;
    lck.l_len = length;
    lck.l_pid = getpid();

    return fcntl(fd, F_SETLKW, &lck);
}

int linda::ProcessFileUtils::readRecord(int fd, process *process_ptr, int record_id) {
    lseek(fd, record_id * (sizeof(process)), 0);
    return read(fd, process_ptr, sizeof(process));
}

int linda::ProcessFileUtils::writeRecord(int fd, process *process_ptr, int record_id) {
    lseek(fd, record_id * (sizeof(process)), 0);
    return write(fd, process_ptr, sizeof(process));
}

int linda::ProcessFileUtils::checkRecordTaken(int fd, int record_id) {
    char flag;
    lseek(fd, record_id * sizeof(process), 0);
    if (int res = read(fd, &flag, sizeof(char)) > 0) {
        return flag;
    }
    else if (res == 0){
        return 0; //end of file => record free
    }
    else {
        return -1; //error
    }
}

int linda::ProcessFileUtils::setRecordTaken(int fd, int record_id, char taken) {
    lseek(fd, record_id * sizeof(process), 0);
    return write(fd, &taken, sizeof(char));
}









