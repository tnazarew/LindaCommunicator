//
// Created by tomasz on 07.06.16.
//

#include <LindaCommunicator.h>
#include <ProcessFileUtils.h>
#include <algorithm>
#include <MatchesFinder.h>
#include <currenttime.h>

linda::LindaCommunicator::LindaCommunicator()
{
    // TODO BLOCK SIGNALS
    // OPEN FILE GET FD
    //fd =
}

void linda::LindaCommunicator::sortQueue(std::vector<ProcessFileUtils::process*> &queue)
{
    std::sort(queue.begin(), queue.end(), linda::compProc);

}

void linda::LindaCommunicator::wakeProcesses(int fd, linda::TupleFileUtils::tuple *tuple)
{
    std::vector<linda::ProcessFileUtils::process*> pids = linda::MatchesFinder::returnProcessQueue(tuple);
    sortQueue(pids);
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
    linda::TupleFileUtils::tuple linda::LindaCommunicator::input(std::string pattern)
    {
        return read_(pattern, true);
    }
    linda::TupleFileUtils::tuple linda::LindaCommunicator::read(std::string pattern)
    {
        return read_(pattern, false);
    }
    void linda::LindaCommunicator::output(std::string tuple)
    {

    }
    linda::TupleFileUtils::tuple linda::LindaCommunicator::read_(std::string pattern, bool input)
    {
        int rec_id = ProcessFileUtils::findAndLock();
        linda::ProcessFileUtils::process proc.initProcess(pattern, input, rec_id);
        ProcessFileUtils::writeRecord(fd, &proc, proc.record_id);
        ProcessFileUtils::unlockRecord(fd, sizeof(proc), proc.record_id);
        TupleFileUtils::tuple* t = MatchesFinder::returnBlockedTuple(&proc);
        ProcessFileUtils::lockRecord(fd, sizeof(proc), proc.record_id);
        if(t)
        {
            if(proc.found)
            {
                proc.taken = false;
                ProcessFileUtils::unlockRecord(fd, sizeof(proc), proc.record_id);
                TupleFileUtils::unlockRecord(fd, sizeof(*t), t->record_id);
            }
//            else
//            {
//
//            }
        }
    }

linda::ProcessFileUtils::process linda::ProcessFileUtils::process::initProcess(const string &pattern, bool input, int rec_id)
{
    linda::ProcessFileUtils::process proc;
    proc.timestamp = get_current_time();
    proc.flag = input;
    proc.found = false;
    proc.pid = getpid();
    proc.record_id = rec_id;
    proc.taken = true;
    strcpy(proc.pattern,pattern.c_str());
}
