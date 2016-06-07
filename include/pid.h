//
// Created by tomasz on 07.06.16.
//

#ifndef LINDACOMMUNICATOR_PID_H
#define LINDACOMMUNICATOR_PID_H

#include <string>

namespace linda
{
    struct Pid
    {
        unsigned long time;
        pid_t pid;
        int imput;
        unsigned int offset;
        std::string pattern;

    public:
        //compare time
        bool operator<(const Pid& other);
    };
}
#endif //LINDACOMMUNICATOR_PID_H
