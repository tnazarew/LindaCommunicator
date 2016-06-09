//
// Created by tomasz on 07.06.16.
//

#include <LindaCommunicator.h>

#include <algorithm>
#include <MatchesFinder.h>


void linda::LindaCommunicator::sortQueue(std::vector<ProccessFileUtils::process> &queue)
{
    std::sort(queue.begin(), queue.end());

}

void linda::LindaCommunicator::wakeProcesses(int fd, linda::TupleFileUtils::tuple *tuple)
{
    std::vector<linda::ProccessFileUtils::process*> pids = linda::MatchesFinder::returnProcessQueue(tuple);
    bool input = false;
    for (ProccessFileUtils::process* pid: pids)
    {
        if(!input)
        {
            int taken = ProccessFileUtils::checkRecordTaken(fd, pid->record_id);
            if(taken == 1)
            {
                ProccessFileUtils::process* ptr;
                ProccessFileUtils::readRecord(fd, ptr, pid->record_id);
                if(ptr->timestamp == pid->timestamp)
                {
                    ptr->found = 1;
                    ProccessFileUtils::writeRecord(fd,  ptr, pid->record_id);
                    int temp_fd = open((DEF_MES_FILE_PREF + std::to_string(pid->pid)).c_str(), O_RDWR|O_CREAT);
                    TupleFileUtils::writeRecord(temp_fd, tuple, tuple->record_id);
                    // TODO - ADD WAKEUP CALL
                    input = ptr->flag?true:input;
                }

            }
        }
        ProccessFileUtils::unlockRecord(fd, sizeof(ProccessFileUtils:: process), pid->record_id);

    }

}