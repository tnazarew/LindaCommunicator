//
// Created by tomasz on 07.06.16.
//

#ifndef LINDACOMMUNICATOR_LINDACOMMUNICATOR_H
#define LINDACOMMUNICATOR_LINDACOMMUNICATOR_H

#include <string>
#include <vector>
#include <pid.h>
#include <tuple.h>
#include <fstream>

namespace linda
{
    static const std::string DEFAULT_FILEPATH = "";
    static const int MAX_TUPLE_SIZE = 0;
    static const std::string DEF_MES_FILE_PREF = "";
    class LindaCommunicator
    {
        int searchFile();
        int insertPID(unsigned int offset);
        int insertTuple(unsigned int offset);
        int removePID(unsigned int offset);
        int removeTuple(unsigned int offset);
        void wakeProcess(pid_t pid);
        void wakeProcesses();
        void sortQueue();
        void findMatching(std::string pattern);
        std::vector<Pid> queue;


    public:
        LindaCommunicator();
        std::fstream tuples, pids, messages;
        std::string read(std::string pattern);
        std::string input(std::string pattern);
        void output(std::string tuple);



    };
}


#endif //LINDACOMMUNICATOR_LINDACOMMUNICATOR_H
