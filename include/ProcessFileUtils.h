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
#include <string>
#include <iostream>

namespace linda
{

    class ProcessFileUtils {
    public:
        struct process {
            bool taken; // taken - 1 | free - 0
            bool flag; // input - 1 | read - 0 ?
            bool found; // default - 0 | waiting for sth - 1
            time_t timestamp;
            pid_t pid;
            int record_id;
            char pattern[99];
            bool operator<(const process& other) const ;
            void initProcess(const std::string& pattern, bool input, int rec_id);
        };

        static int wakeupProcess(pid_t pid);

        static int lockRecord(int fd, int length, int record_id);

        static int unlockRecord(int fd, int length, int record_id);

        static int readRecord(int fd, ProcessFileUtils:: process *process_ptr, int record_id);

        static int writeRecord(int fd, ProcessFileUtils:: process *process_ptr, int record_id);

        static int checkRecordTaken(int fd, int record_id);

        static int setRecordTaken(int fd, int record_id, char taken);

        static int findAndLock();


    };

    bool compProc(ProcessFileUtils::process* a, ProcessFileUtils::process*b);
}

#endif //LINDACOMMUNICATOR_PROCCESSFILEUTILS_H
