//
// Created by paulina on 09.06.16.
//

#include <linda_exception.h>
#include "MatchesFinder.h"
#include "PatternComparator.h"

linda::TupleFileUtils::tuple *linda::MatchesFinder::returnBlockedTuple(int fd, linda::ProcessFileUtils::process *process) {


    if(fd == -1)
        throw linda::LindaException("");
    int index = 0;
    TupleFileUtils t;
    TupleFileUtils::tuple tu2;
    while(true) {
        t.lockRecord(fd, sizeof(struct TupleFileUtils::tuple), index);
        int read = t.readRecord(fd, &tu2, index);
        if(read == 0 || read == -1) {
            t.unlockRecord(fd, sizeof(struct TupleFileUtils::tuple), index);
            break;
        }
        if(tu2.taken == 0) {
            ++index;
            continue;
        }
        std::string pattern(tu2.pattern);
        if (PatternComparator::matches(pattern, process->pattern))
            return std::move(&tu2);
        t.unlockRecord(fd, sizeof(struct TupleFileUtils::tuple), index);
        ++index;
    }
    return nullptr;/*
    TupleFileUtils t;
    TupleFileUtils::tuple tu, tu2;
    t.lockRecord(lfd, sizeof(struct TupleFileUtils::tuple), 0);
    t.readRecord(lfd, &tu2, 0);
    t.unlockRecord(lfd, sizeof(struct TupleFileUtils::tuple), 0);*/
}

std::vector<linda::ProcessFileUtils::process *> linda::MatchesFinder::returnProcessQueue(int fd, linda::TupleFileUtils::tuple *tuple) {


    if(fd == -1)
        throw linda::LindaException("");
    std::vector<ProcessFileUtils::process *> processes;
    int index = 0;
    ProcessFileUtils t;
    ProcessFileUtils::process proc;
    while(true) {
        t.lockRecord(fd, sizeof(struct ProcessFileUtils::process), index);
        int read = t.readRecord(fd, &proc, index);
        if(read == 0 || read == -1) {
            t.unlockRecord(fd, sizeof(struct ProcessFileUtils::process), index);
            break;
        }
        if(proc.taken == 0 || proc.found == 1) {
            ++index;
            continue;
        }
        std::string pattern(proc.pattern);
        if (PatternComparator::matches(tuple->pattern, pattern))
            processes.push_back(std::move(&proc));
        else
            t.unlockRecord(fd, sizeof(struct ProcessFileUtils::process), index);
        ++index;
    }
    return std::move(processes);
}



