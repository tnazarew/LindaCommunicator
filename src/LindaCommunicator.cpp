//
// Created by tomasz on 07.06.16.
//

#include <LindaCommunicator.h>

#include <algorithm>
#include <MatchesFinder.h>


void linda::LindaCommunicator::sortQueue(std::vector<ProcessFileUtils::process> &queue)
{
    std::sort(queue.begin(), queue.end());

}

void linda::LindaCommunicator::wakeProcesses(int fd, linda::TupleFileUtils::tuple *tuple)
{
    std::vector<linda::ProcessFileUtils::process*> pids = linda::MatchesFinder::returnProcessQueue(tuple);
    bool input = false;
    for (ProcessFileUtils::process* pid: pids)
    {
        if(!input)
        {
            int taken = ProcessFileUtils::checkRecordTaken(fd, pid->record_id);
            if(taken == 1)
            {
                ProcessFileUtils::process* ptr;
                ProcessFileUtils::readRecord(fd, ptr, pid->record_id);
                if(ptr->timestamp == pid->timestamp)
                {
                    ptr->found = 1;
                    ProcessFileUtils::writeRecord(fd,  ptr, pid->record_id);
                    int temp_fd = open((DEF_MES_FILE_PREF + std::to_string(pid->pid)).c_str(), O_RDWR|O_CREAT);
                    TupleFileUtils::writeRecord(temp_fd, tuple, tuple->record_id);
                    // TODO - ADD WAKEUP CALL
                    input = ptr->flag?true:input;
                }

            }
        }
        ProcessFileUtils::unlockRecord(fd, sizeof(ProcessFileUtils:: process), pid->record_id);

    }

}