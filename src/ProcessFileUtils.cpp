#include <signal.h>
#include <bits/stringfwd.h>
#include <SearchProcessFile.h>
#include <linda_exception.h>

#include "ProcessFileUtils.h"



int  linda::ProcessFileUtils::lockRecord(int fd, int length, int record_id) {
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
    int res = fcntl(fd, F_SETLKW, &lck);
    if(res == -1)
        throw linda::LindaException();
    return res;
}

int linda::ProcessFileUtils::readRecord(int fd, process *process_ptr, int record_id) {
    const __off_t i = lseek(fd, record_id * (sizeof(process)), 0);
    if(i == -1)
        throw linda::LindaException();
    int res = read(fd, process_ptr, sizeof(process));
    if(res == -1)
        throw linda::LindaException();
    return res;
}

int linda::ProcessFileUtils::writeRecord(int fd, process *process_ptr, int record_id) {
    const __off_t i = lseek(fd, record_id * (sizeof(process)), 0);
    if(i == -1)
        throw linda::LindaException();
    int res = write(fd, process_ptr, sizeof(process));
    if(res == -1)
        throw linda::LindaException();
    return res;
}

int linda::ProcessFileUtils::checkRecordTaken(int fd, int record_id) {
    char flag;
    const __off_t i = lseek(fd, record_id * sizeof(process), 0);
    if(i ==-1)
        throw linda::LindaException();
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

int linda::ProcessFileUtils::setRecordTaken(int fd, int record_id, char taken)
{
    const __off_t i = lseek(fd, record_id * sizeof(process), 0);
    if(i == -1)
        throw linda::LindaException();
    const ssize_t i1 = write(fd, &taken, sizeof(char));
    if(i1 == -1)
    return i1;
}

int linda::ProcessFileUtils::wakeupProcess(pid_t pid)
{
    const int i = kill(pid, SIGUSR1);
    if(i == -1)
        throw linda::LindaException();
    return i;
}

bool linda::compProc(ProcessFileUtils::process* a, ProcessFileUtils::process*b)
{
    return *a < *b;
}

bool linda::ProcessFileUtils::process::operator<(const process &other) const
{
    return this->timestamp < other.timestamp;
}

int linda::ProcessFileUtils::findAndLock(int fd)
{

    int rec_id=0;
    while(true)
    {
        ProcessFileUtils::lockRecord(fd, sizeof(ProcessFileUtils::process), rec_id);
        if(!ProcessFileUtils::checkRecordTaken(fd, rec_id))
        {
            return rec_id;
        }
        ProcessFileUtils::unlockRecord(fd, sizeof(ProcessFileUtils::process), rec_id);
        rec_id ++;
    }
}

