#include <iostream>

#include <fcntl.h>
#include <PatternComparator.h>
#include "ProcessFileUtils.h"
#include <TupleFileUtils.h>
#include <MatchesFinder.h>
#include <LindaCommunicator.h>
#include <linda_exception.h>

using namespace linda;

int main(int argc, char** argv)
{
    if(argc > 1)
    {
        linda::LindaCommunicator com("tuple_test", "proc_test");
        linda::TupleFileUtils::tuple t;
        std::string first(argv[1]);
        try
        {
            if(first == "input")
            {
                t = com.input(std::string(argv[2]));
                std::cout << t.pattern << std::endl;
            }
            else if(first == "output")
                com.output(std::string(argv[2]));
            else if(first == "read")
            {
                t = com.read(std::string(argv[2]));
                std::cout << t.pattern << std::endl;
            }
        }
        catch (LindaException& l)
        {
            std::cerr << l.what();
            return -1;
        }
        catch (...)
        {
            std::cerr << "Unknown but very bad error";
        }

        std::cout<<std::endl;
    }
    int jou = 0;
    return 0;
}
