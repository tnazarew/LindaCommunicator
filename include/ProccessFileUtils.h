#ifndef LINDACOMMUNICATOR_PROCCESSFILEUTILS_H
#define LINDACOMMUNICATOR_PROCCESSFILEUTILS_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <fcntl.h>
#include <fcntl.h>
#include <errno.h>

namespace linda
{
    class ProccessFileUtils {
    public:
        struct process {
            char taken; // taken - 1 | free - 0
            char flag; // input - 1 | read - 0 ?
            char found; // default - 0 | waiting for sth - 1
            time_t timestamp;
            pid_t pid;
            int record_id;
            char pattern[99];
            bool operator<(const process& other) const ;
        };

        ProccessFileUtils() {};

        static int lockRecord(int fd, int length, int record_id);

        static int unlockRecord(int fd, int length, int record_id);

        static int readRecord(int fd, ProccessFileUtils:: process *process_ptr, int record_id);

        static int writeRecord(int fd, ProccessFileUtils:: process *process_ptr, int record_id);

        static int checkRecordTaken(int fd, int record_id);

        static int setRecordTaken(int fd, int record_id, char taken);
    };

}

#endif //LINDACOMMUNICATOR_PROCCESSFILEUTILS_H
