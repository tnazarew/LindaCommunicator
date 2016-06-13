#include <iostream>

#include <fcntl.h>
#include <PatternComparator.h>
#include "ProcessFileUtils.h"
#include <TupleFileUtils.h>
#include <MatchesFinder.h>
#include <LindaCommunicator.h>

using namespace linda;

int main(int argc, char** argv)
{
    if(argc > 1)
    {
        linda::LindaCommunicator com("tuple_test", "proc_test");
        linda::TupleFileUtils::tuple t;
        std::string first(argv[1]);
        if(first == "input")
            t = com.input(std::string(argv[2]));
        else if(first == "output")
            com.output(std::string(argv[2]));
        else if(first == "read")
            t = com.read(std::string(argv[2]));
    }
    return 0;
}
