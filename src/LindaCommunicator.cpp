//
// Created by tomasz on 07.06.16.
//

#include "LindaCommunicator.h"
#include "ProccessFileUtils.h"

#include <pid.h>
#include <algorithm>
void linda::LindaCommunicator::sortQueue(std::vector<Pid> &queue)
{
    std::sort(queue.begin(), queue.end());

}

void linda::LindaCommunicator::wakeProcesses(int fd, std::vector<struct process> pids)
{
    bool input = false;
    for (Pid pid: pids)
    {
        if(!input)
        {
            ProccessFileUtils::lockRecord(fd,sizeof(struct process), pid.record_id);
            int a = ProccessFileUtils::checkRecordTaken(fd, pid.record_id);
            if(a == 1)
            {
                struct process* ptr;
                ProccessFileUtils::readRecord(fd, ptr, pid.record_id);
                if(ptr->timestamp == pid.timestamp)
                {
                    ptr->found = 1;
                    int writeRecord(int fd, struct process *process_ptr, int record_id);
                    input = input?true:input;
                }

            }
        }
        ProccessFileUtils::unlockRecord(fd, sizeof(struct process), pid.record_id);

    }

}