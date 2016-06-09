//
// Created by tomasz on 09.06.16.
//

#include <pid.h>

bool linda::Pid::operator<(const Pid& other)
{
    return this->time > other.time;
}