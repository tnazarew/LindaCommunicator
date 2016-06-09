//
// Created by paulina on 09.06.16.
//

#ifndef LINDACOMMUNICATOR_MATCHESFINDER_H
#define LINDACOMMUNICATOR_MATCHESFINDER_H


#include <string>
#include <vector>
#include "TupleFileUtils.h"
#include "ProcessFileUtils.h"

namespace linda {

    class MatchesFinder {
    public:
        static TupleFileUtils::tuple* returnBlockedTuple(ProcessFileUtils::process* process );
        static std::vector<ProcessFileUtils::process *> returnProcessQueue(TupleFileUtils::tuple* tuple);
    };
}


#endif //LINDACOMMUNICATOR_MATCHESFINDER_H
