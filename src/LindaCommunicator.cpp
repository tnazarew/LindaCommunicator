//
// Created by tomasz on 07.06.16.
//

#include <LindaCommunicator.h>

#include <algorithm>
#include <MatchesFinder.h>
#include <currenttime.h>
#include <signals.h>

#include <signal.h>
#include <linda_exception.h>

linda::LindaCommunicator::LindaCommunicator(std::string tuple_file, std::string process_file)
{
    tuple_fd = open((DEFAULT_FILEPATH+tuple_file).c_str(), O_RDWR|O_CREAT, S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH);
    proc_fd = open((DEFAULT_FILEPATH+process_file).c_str(), O_RDWR|O_CREAT, S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH);
    //blokowanie sygnałów
//    std::cout << getpid() << std::endl;
    sigset_t block_set;

    if(sigemptyset(&block_set) == -1)
        throw linda::LindaException("Failed to empty set");

    if(sigaddset(&block_set, SIGUSR1) == -1)
        throw linda::LindaException("Failed to add int signal");

    if(sigprocmask(SIG_BLOCK, &block_set, NULL) == -1)
        throw linda::LindaException("Failed to block SIGINT");

}

linda::LindaCommunicator::~LindaCommunicator()
{
    if(close(tuple_fd)==-1)
        throw linda::LindaException("");
    if(close(proc_fd)==-1)
        throw linda::LindaException("");
}

void linda::LindaCommunicator::sortQueue(std::vector<ProcessFileUtils::process> &queue)
{
    std::sort(queue.begin(), queue.end());

}

void linda::LindaCommunicator::wakeProcesses(linda::TupleFileUtils::tuple *tuple)
{
    std::vector<linda::ProcessFileUtils::process> pids = linda::MatchesFinder::returnProcessQueue(proc_fd, tuple);
    sortQueue(pids);
    bool input = false;
    for (ProcessFileUtils::process pid: pids) {
        if(!input) {
            ProcessFileUtils::process ptr;
            ProcessFileUtils::readRecord(proc_fd, &ptr, pid.record_id);
            ptr.found = 1;
            ProcessFileUtils::writeRecord(proc_fd, &ptr, pid.record_id);
            ProcessFileUtils::unlockRecord(proc_fd, sizeof(ptr), ptr.record_id);
            int temp_fd = open((DEFAULT_FILEPATH + DEF_MES_FILE_PREF + std::to_string(pid.pid)).c_str(), O_RDWR | O_CREAT,
                               S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH);

            if (close(temp_fd) == -1) {
                TupleFileUtils::unlockRecord(tuple_fd, sizeof(*tuple), tuple->record_id);
                throw linda::LindaException("");
            }

            ProcessFileUtils::wakeupProcess(pid.pid);
            if (ptr.flag) {
                input = true;
            }
        }
        else
        {
            ProcessFileUtils::unlockRecord(proc_fd, sizeof(pid), pid.record_id);
        }
    }
    TupleFileUtils::unlockRecord(tuple_fd, sizeof(*tuple), tuple->record_id);
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
    int rec_id = TupleFileUtils::findAndLock(tuple_fd);
    TupleFileUtils::tuple t;
    t.record_id = rec_id;
    tuple.copy(t.pattern, tuple.size());
    t.pattern[tuple.size()] = '\0';
    t.taken = true;

    TupleFileUtils::writeRecord(tuple_fd, &t, rec_id);

    wakeProcesses(&t);


}

linda::TupleFileUtils::tuple linda::LindaCommunicator::read_(std::string pattern, bool input)
{
    int rec_id = ProcessFileUtils::findAndLock(proc_fd); //wolne miejsce w pliku z procesami zakładamy że blokure obszar
    linda::ProcessFileUtils::process proc;
    proc.initProcess(pattern, input, rec_id); // towrzymy strukture
    ProcessFileUtils::writeRecord(proc_fd, &proc, proc.record_id); //wpisuje w obszar
    ProcessFileUtils::unlockRecord(proc_fd, sizeof(proc), proc.record_id); //zwlania
    TupleFileUtils::tuple t = MatchesFinder::returnBlockedTuple(tuple_fd, &proc); // znajduje krotke dla wybranego procesu
    ProcessFileUtils::lockRecord(proc_fd, sizeof(proc), proc.record_id); // blokuje obszar
    if (t.record_id != -1) // krotka znaleziona
    {
        if (proc.found) //inny proces znalazł juz krotke dla naszego procesu
        {
            proc.taken = false;
            ProcessFileUtils::writeRecord(proc_fd, &proc, proc.record_id);
            ProcessFileUtils::unlockRecord(proc_fd, sizeof(proc), proc.record_id);
            TupleFileUtils::unlockRecord(tuple_fd, sizeof(t), t.record_id);
            linda::ExecuteThisThingThatWillSuspendProcesUntilItGetsSIGUSR1Signal();
        }
        else // nie znaleziono dla nas krotki, nasza jest tą jedyną
        {
            if (proc.flag) // input
            {
                t.taken = false;
                TupleFileUtils::writeRecord(tuple_fd, &t, t.record_id);
            }
            proc.taken = false;
            ProcessFileUtils::writeRecord(proc_fd, &proc, proc.record_id);
            ProcessFileUtils::unlockRecord(proc_fd, sizeof(proc), proc.record_id);
            TupleFileUtils::unlockRecord(tuple_fd, sizeof(t), t.record_id);
            return t;
        }
    }
    else
    {
        ProcessFileUtils::unlockRecord(proc_fd, sizeof(proc), proc.record_id);

        linda::ExecuteThisThingThatWillSuspendProcesUntilItGetsSIGUSR1Signal();
    }
    int new_fd = open((DEFAULT_FILEPATH + DEF_MES_FILE_PREF + std::to_string(proc.pid)).c_str(),O_RDWR , S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH);
    if(new_fd == -1)
        throw linda::LindaException("Unable to open auxiliary file");

    ProcessFileUtils::lockRecord(proc_fd, sizeof(proc), rec_id);
    ProcessFileUtils::setRecordTaken(proc_fd, rec_id, false);
    ProcessFileUtils::unlockRecord(proc_fd, sizeof(proc), rec_id);

    TupleFileUtils::tuple mes_t;
    TupleFileUtils::readRecord(new_fd, &mes_t, 0);


    return mes_t;
}

void linda::ProcessFileUtils::process::initProcess(const string &pattern_, bool input, int rec_id)
{
    timestamp = get_current_time();
    flag = input;
    found = false;
    pid = getpid();
    if(pid == -1)
        throw linda::LindaException("Getpid failed");
    record_id = rec_id;
    taken = true;
    pattern_.copy(pattern, pattern_.size());
    pattern[pattern_.size()] = '\0';
    std::cout << pattern << std::endl;
}


