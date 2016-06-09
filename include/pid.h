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
        int empty;
        int found;
        int input;
        int record_id;
        std::string pattern;

    public:
        //compare time
        bool operator<(Pid& other);
    };
}
#endif //LINDACOMMUNICATOR_PID_H
