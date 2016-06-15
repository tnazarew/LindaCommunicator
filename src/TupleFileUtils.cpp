#include <linda_exception.h>
#include <iostream>
#include "TupleFileUtils.h"

int linda::TupleFileUtils::lockRecord(int fd, int length, int record_id)
{
    struct flock lck;
    lck.l_type = F_WRLCK;
    lck.l_whence = 0;
    lck.l_start = record_id * length;
    lck.l_len = length;
    const __pid_t t = lck.l_pid = getpid();
    if(t==-1)
        throw linda::LindaException("");

    const int i = fcntl(fd, F_SETLKW, &lck);
    if(i == -1)
        throw linda::LindaException(std::string("TupleFileUtils::lockRecord fcntl failed| Errno: ") + strerror(errno));
    std::cout << "tuple lock: " <<  record_id << std::endl;
    return i;
}

int linda::TupleFileUtils::unlockRecord(int fd, int length, int record_id)
{
    struct flock lck;
    lck.l_type = F_UNLCK;
    lck.l_whence = 0;
    lck.l_start = record_id * length;
    lck.l_len = length;
    const __pid_t t = lck.l_pid = getpid();
    if(t==-1)
        throw linda::LindaException("");
    std::cout << "tuple unlock: " <<  record_id << std::endl;
    return fcntl(fd, F_SETLKW, &lck);
}

int linda::TupleFileUtils::readRecord(int fd, tuple *tuple_ptr, int record_id)
{
    const __off_t i = lseek(fd, record_id * (sizeof(tuple)), 0);
    if(i== -1)
        throw linda::LindaException("");
    const ssize_t i1 = read(fd, tuple_ptr, sizeof(struct tuple));
    if(i1 == -1)
        throw linda::LindaException("");
    return i1;
}

int linda::TupleFileUtils::writeRecord(int fd, tuple *tuple_ptr, int record_id)
{
    const __off_t i = lseek(fd, record_id * (sizeof(tuple)), 0);
    if(i ==-1)
        throw linda::LindaException("");
    const ssize_t i1 = write(fd, tuple_ptr, sizeof(tuple));
    if(i1 == -1)
        throw linda::LindaException("");
    return i1;
}

int linda::TupleFileUtils::checkRecordTaken(int fd, int record_id)
{
    char flag;
    lseek(fd, record_id * sizeof(tuple), 0);
    if (int res = read(fd, &flag, sizeof(char)) > 0)
    {
        return flag;
    }
    else if (res == 0)
    {
        return 0; //end of file => record free
    }
    else
    {
        return -1; //error
    }
}

int linda::TupleFileUtils::setRecordTaken(int fd, int record_id, int taken)
{
    const __off_t i = lseek(fd, record_id * sizeof(struct tuple), 0);
    if(i==-1)
        throw linda::LindaException("");
    const ssize_t i1 = write(fd, &taken, sizeof(int));
    if(i1 == -1)
        throw linda::LindaException("");
    return i1;
}

int linda::TupleFileUtils::findAndLock(int fd)
{
    int rec_id = 0;
    int first = 0;
    while (true)
    {
        TupleFileUtils::lockRecord(fd, sizeof(TupleFileUtils::tuple), rec_id);
        if (!TupleFileUtils::checkRecordTaken(fd, rec_id))
        {
            if (!first)
                TupleFileUtils::unlockRecord(fd, sizeof(TupleFileUtils::tuple), rec_id-1);
            return rec_id;
        }
        if (!first)
            TupleFileUtils::unlockRecord(fd, sizeof(TupleFileUtils::tuple), rec_id-1);
        else
            first = 1;

        rec_id++;
    }
}





