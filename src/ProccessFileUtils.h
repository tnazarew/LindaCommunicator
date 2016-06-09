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
            char taken;
            char flag;
            time_t timestamp;
            pid_t pid;
            char pattern[99];
        };

        ProccessFileUtils() {};

        int lockRecord(int fd, int length, int record_id);

        int unlockRecord(int fd, int length, int record_id);

        int readRecord(int fd, struct process *process_ptr, int record_id);

        int writeRecord(int fd, struct process *process_ptr, int record_id);

        int checkRecordTaken(int fd, int record_id);

        int setRecordTaken(int fd, int record_id, char taken);
    };

}

#endif //LINDACOMMUNICATOR_PROCCESSFILEUTILS_H
