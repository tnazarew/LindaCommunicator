//
// Created by tomasz on 07.06.16.
//

#ifndef LINDACOMMUNICATOR_LINDACOMMUNICATOR_H
#define LINDACOMMUNICATOR_LINDACOMMUNICATOR_H

#include <string>
#include <vector>
#include <tuple.h>
#include <fstream>
#include <ProcessFileUtils.h>
#include <TupleFileUtils.h>
namespace linda
{
    static const std::string DEFAULT_FILEPATH = "/tmp/";
    static const int MAX_TUPLE_SIZE = 0;
    static const std::string DEF_MES_FILE_PREF = "lin_mes_";
    class LindaCommunicator
    {
        int searchFile();
        int insertPID(unsigned int offset);
        int insertTuple(unsigned int offset);
        int removePID(unsigned int offset);
        int removeTuple(unsigned int offset);
        void wakeProcess(pid_t pid);
        void wakeProcesses(linda::TupleFileUtils::tuple *tuple);
        void sortQueue(std::vector<ProcessFileUtils::process>&);
        void findMatching(std::string pattern);
        TupleFileUtils::tuple read_(std::string, int);
        TupleFileUtils::tuple readWhenOtherProcessFound(ProcessFileUtils::process &proc, TupleFileUtils::tuple &t);
        TupleFileUtils::tuple readWhenIFound(ProcessFileUtils::process &proc, TupleFileUtils::tuple &t);
        TupleFileUtils::tuple readWhenNobodyFound(ProcessFileUtils::process &proc);



        int tuple_fd, proc_fd;


    public:
        LindaCommunicator(std::string tuple_file, std::string proc_file);
        ~LindaCommunicator();
        TupleFileUtils::tuple read(std::string pattern);
        TupleFileUtils::tuple input(std::string pattern);
        void output(std::string tuple);



    };
}


#endif //LINDACOMMUNICATOR_LINDACOMMUNICATOR_H
