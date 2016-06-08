#ifndef LINDACOMMUNICATOR_SEARCHPROCESSFILE_H
#define LINDACOMMUNICATOR_SEARCHPROCESSFILE_H

#include "ProcessFileUtils.h"

namespace linda {

    class SearchProcessFile {
    public:
        static int putProcess(int fd, ProcessFileUtils::process * processPtr);
    };

}
#endif //LINDACOMMUNICATOR_SEARCHPROCESSFILE_H
