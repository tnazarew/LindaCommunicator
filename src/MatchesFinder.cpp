//
// Created by paulina on 09.06.16.
//

#include <linda_exception.h>
#include "MatchesFinder.h"
#include "PatternComparator.h"

linda::TupleFileUtils::tuple linda::MatchesFinder::returnBlockedTuple(int fd, linda::ProcessFileUtils::process *process) {


    if(fd == -1)
        throw linda::LindaException("MatchesFinder::returnBlockedTuple: " + std::string(strerror(errno)));
    int index = 0;
    TupleFileUtils::tuple tu2;
    while(true) {
        TupleFileUtils::lockRecord(fd, sizeof(struct TupleFileUtils::tuple), index);
        int read = TupleFileUtils::readRecord(fd, &tu2, index);
        if(read == 0 || read == -1) {
            TupleFileUtils::unlockRecord(fd, sizeof(struct TupleFileUtils::tuple), index);
            break;
        }
        if(tu2.taken == 0)
        {
            TupleFileUtils::unlockRecord(fd, sizeof(TupleFileUtils::tuple), index);
            ++index;
            continue;
        }
        std::string pattern(tu2.pattern);
        if (PatternComparator::matches(pattern, process->pattern))
            return tu2;
        TupleFileUtils::unlockRecord(fd, sizeof(struct TupleFileUtils::tuple), index);
        ++index;
    }
    TupleFileUtils::tuple tt;
    tt.taken = false;
    tt.record_id = -1;
    return tt;
}

std::vector<linda::ProcessFileUtils::process> linda::MatchesFinder::returnProcessQueue(int fd,
                                                                                         linda::TupleFileUtils::tuple *tuple) {


    if(fd == -1)
        throw linda::LindaException("MatchesFinder::returnProcessQueue: "+ std::string(strerror(errno)));
    std::vector<ProcessFileUtils::process> processes;
    int index = 0;
    ProcessFileUtils::process proc;
    while(true) {
        ProcessFileUtils::lockRecord(fd, sizeof(struct ProcessFileUtils::process), index);
        int read = ProcessFileUtils::readRecord(fd, &proc, index);
        if(read == 0 || read == -1) {
            ProcessFileUtils::unlockRecord(fd, sizeof(struct ProcessFileUtils::process), index);
            break;
        }
        if(proc.taken == 0 || proc.found == 1) {
            ProcessFileUtils::unlockRecord(fd, sizeof(struct ProcessFileUtils::process), index);
            ++index;
            continue;
        }

        if (PatternComparator::matches(tuple->pattern, proc.pattern))
            processes.push_back(proc);
        else
            ProcessFileUtils::unlockRecord(fd, sizeof(struct ProcessFileUtils::process), index);
        ++index;
    }
    return processes;
}



