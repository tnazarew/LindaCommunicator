#ifndef LINDACOMMUNICATOR_TUPLEFILEUTILS_H
#define LINDACOMMUNICATOR_TUPLEFILEUTILS_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <fcntl.h>
#include <fcntl.h>
#include <errno.h>

namespace linda {
    class TupleFileUtils {
    public:
        struct tuple {
            char taken;
            char pattern[99];
        };
        static int lockRecord(int fd, int length, int record_id);

        static int unlockRecord(int fd, int length, int record_id);

        static int readRecord(int fd, tuple *tuple_ptr, int record_id);

        static int writeRecord(int fd, tuple *tuple_ptr, int record_id);

        static int checkRecordTaken(int fd, int record_id);

        static int setRecordTaken(int fd, int record_id, char taken);
    };
}


#endif //LINDACOMMUNICATOR_TUPLEFILEUTILS_H
