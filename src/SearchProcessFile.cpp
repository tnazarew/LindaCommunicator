#include <SearchProcessFile.h>
#include <ProcessFileUtils.h>

/* Puts process in a file and returns with locked record.
 * Returns record id or -1 in case of failure*/
namespace linda
{
    int SearchProcessFile::putProcess(int fd, ProcessFileUtils::process * processPtr) {
        for (int i = 0; true; i++) {

            if (ProcessFileUtils::lockRecord(fd, sizeof(ProcessFileUtils::process), i) != 0)
                return -1;

            if (int res = ProcessFileUtils::checkRecordTaken(fd, i) == 0) {
                //res == 0 => free record or end of file
                if (ProcessFileUtils::writeRecord(fd, processPtr, i) > 0)
                    return i;
                else
                    return -1;
            }
            else if (res == -1) {
                //checking record taken failed
                ProcessFileUtils::unlockRecord(fd, sizeof(ProcessFileUtils::process), i);
                return -1;
            }

            if (ProcessFileUtils::unlockRecord(fd, sizeof(ProcessFileUtils::process), i) != 0)
                return -1;
        }
    }
}

