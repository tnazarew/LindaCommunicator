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
        struct tuple {
            char taken;
            char pattern[99];
        };

        int lockRecord(int fd, int length, int record_id);

        int unlockRecord(int fd, int length, int record_id);

        int readRecord(int fd, struct tuple *tuple_ptr, int record_id);

        int writeRecord(int fd, struct tuple *tuple_ptr, int record_id);

        int checkRecordTaken(int fd, int record_id);
    };
}


#endif //LINDACOMMUNICATOR_TUPLEFILEUTILS_H
