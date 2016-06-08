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
    class ProcessFileUtils {

    public:
        struct process {
            char taken;
            char flag;
            time_t timestamp;
            pid_t pid;
            char pattern[99];
        };

        static int lockRecord(int fd, int length, int record_id);

        static int unlockRecord(int fd, int length, int record_id);

        static int readRecord(int fd, process *process_ptr, int record_id);

        static int writeRecord(int fd, process *process_ptr, int record_id);

        static int checkRecordTaken(int fd, int record_id);

        static int setRecordTaken(int fd, int record_id, char taken);
    };
}

#endif //LINDACOMMUNICATOR_PROCCESSFILEUTILS_H
