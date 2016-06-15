#include <LindaCommunicator.h>

#include <algorithm>
#include <MatchesFinder.h>
#include <currenttime.h>
#include <signals.h>

#include <signal.h>
#include <linda_exception.h>
//*********************************************************************************************************************
void sigusr1Handler(int, siginfo_t*, void*) { }

linda::LindaCommunicator::LindaCommunicator(std::string tuple_file, std::string process_file)
{
    tuple_fd = open((DEFAULT_FILEPATH+tuple_file).c_str(), O_RDWR | O_CREAT, S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH);
    proc_fd = open((DEFAULT_FILEPATH+process_file).c_str(), O_RDWR | O_CREAT, S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH);


    // Set SIGUSR1 handler
    struct sigaction act;
    memset(&act, 0, sizeof(act));
    act.sa_sigaction = sigusr1Handler;
    act.sa_flags = SA_SIGINFO;
    sigaction(SIGUSR1, &act, NULL);


    sigset_t block_set;

    if(sigemptyset(&block_set) == -1)
        throw linda::LindaException("Failed to empty set");

    if(sigaddset(&block_set, SIGUSR1) == -1)
        throw linda::LindaException("Failed to add int signal");

    if(sigprocmask(SIG_BLOCK, &block_set, NULL) == -1)
        throw linda::LindaException("Failed to block SIGUSR1");

}
//*********************************************************************************************************************
linda::LindaCommunicator::~LindaCommunicator()
{
    close(tuple_fd);
    close(proc_fd);
}
//*********************************************************************************************************************
void linda::LindaCommunicator::sortQueue(std::vector<ProcessFileUtils::process> &queue)
{
    std::sort(queue.begin(), queue.end());
}
//*********************************************************************************************************************
void linda::LindaCommunicator::wakeProcesses(linda::TupleFileUtils::tuple *tuple)
{
    std::vector<linda::ProcessFileUtils::process> pids = linda::MatchesFinder::returnProcessQueue(proc_fd, tuple);
    sortQueue(pids);
    int input = 0;
    for (ProcessFileUtils::process pid : pids) {
        if(!input) {
            ProcessFileUtils::process ptr;
            ProcessFileUtils::readRecord(proc_fd, &ptr, pid.record_id);
            ptr.found = 1;
            ProcessFileUtils::writeRecord(proc_fd, &ptr, pid.record_id);
            ProcessFileUtils::unlockRecord(proc_fd, sizeof(ptr), ptr.record_id);

            int temp_fd = open((DEFAULT_FILEPATH + DEF_MES_FILE_PREF + std::to_string(pid.pid)).c_str(), O_RDWR | O_CREAT,
                               S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH);

            TupleFileUtils::writeRecord(temp_fd, tuple, 0);

            if (close(temp_fd) == -1) {
                TupleFileUtils::unlockRecord(tuple_fd, sizeof(*tuple), tuple->record_id);
                throw linda::LindaException("");
            }

            ProcessFileUtils::wakeupProcess(pid.pid);
            if (ptr.flag)
            {
                TupleFileUtils::setRecordTaken(tuple_fd, tuple->record_id, 0);
                input = 1;
            }
        }
        else
        {
            ProcessFileUtils::unlockRecord(proc_fd, sizeof(pid), pid.record_id);
        }
    }
    TupleFileUtils::unlockRecord(tuple_fd, sizeof(*tuple), tuple->record_id);

    std::cerr << "AWOKEN " << pids.size() << std::endl;
}
//*********************************************************************************************************************
linda::TupleFileUtils::tuple linda::LindaCommunicator::input(std::string pattern)
{
    return read_(pattern, 1);
}
//*********************************************************************************************************************
linda::TupleFileUtils::tuple linda::LindaCommunicator::read(std::string pattern)
{
    return read_(pattern, 0);
}
//*********************************************************************************************************************
void linda::LindaCommunicator::output(std::string tuple)
{
    int rec_id = TupleFileUtils::findAndLock(tuple_fd);
    TupleFileUtils::tuple t;
    t.record_id = rec_id;
    tuple.copy(t.pattern, tuple.size());
    t.pattern[tuple.size()] = '\0';
    t.taken = 1;

    TupleFileUtils::writeRecord(tuple_fd, &t, rec_id);

    wakeProcesses(&t);
}
//*********************************************************************************************************************
linda::TupleFileUtils::tuple linda::LindaCommunicator::read_(std::string pattern, int input)
{
    int rec_id = ProcessFileUtils::findAndLock(proc_fd); // wolne miejsce w pliku z procesami zakładamy że blokuje obszar
    linda::ProcessFileUtils::process proc;
    proc.initProcess(pattern, input, rec_id); // tworzymy strukture
    ProcessFileUtils::writeRecord(proc_fd, &proc, proc.record_id); // wpisuje w obszar
    ProcessFileUtils::unlockRecord(proc_fd, sizeof(proc), proc.record_id); // zwalnia

    TupleFileUtils::tuple t = MatchesFinder::returnBlockedTuple(tuple_fd, &proc); // znajduje krotke dla wybranego procesu
    ProcessFileUtils::lockRecord(proc_fd, sizeof(proc), proc.record_id); // blokuje obszar

    if (t.record_id != -1) // krotka znaleziona
    {
        if (proc.found) // inny proces znalazł juz krotke dla naszego procesu
        {
            return readWhenOtherProcessFound(proc, t);
        }

        else // nie znaleziono dla nas krotki, nasza jest tą jedyną
        {
            return readWhenIFound(proc, t);
        }
    }

    else
    {
        return readWhenNobodyFound(proc);
    }
}
//*********************************************************************************************************************
linda::TupleFileUtils::tuple linda::LindaCommunicator::readWhenOtherProcessFound(ProcessFileUtils::process &proc, TupleFileUtils::tuple &t)
{
    std::cerr<<"OTHER" << std::endl;
    proc.taken = 0;
    ProcessFileUtils::writeRecord(proc_fd, &proc, proc.record_id);
    ProcessFileUtils::unlockRecord(proc_fd, sizeof(proc), proc.record_id);
    TupleFileUtils::unlockRecord(tuple_fd, sizeof(t), t.record_id);
    linda::sigusr1Suspend();

    int new_fd = open((DEFAULT_FILEPATH + DEF_MES_FILE_PREF + std::to_string(proc.pid)).c_str(), O_RDWR, S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH);
    if(new_fd == -1)
        throw linda::LindaException("Unable to open auxiliary file");
    TupleFileUtils::tuple mes_t;
    TupleFileUtils::readRecord(new_fd, &mes_t, 0);

    return mes_t;
}
//*********************************************************************************************************************
linda::TupleFileUtils::tuple linda::LindaCommunicator::readWhenIFound(ProcessFileUtils::process &proc, TupleFileUtils::tuple &t)
{
    std::cerr<<"I FOUND" << std::endl;
    if (proc.flag) // input
    {
        t.taken = 0;
        TupleFileUtils::writeRecord(tuple_fd, &t, t.record_id);
    }
    proc.taken = 0;
    ProcessFileUtils::writeRecord(proc_fd, &proc, proc.record_id);
    ProcessFileUtils::unlockRecord(proc_fd, sizeof(proc), proc.record_id);
    TupleFileUtils::unlockRecord(tuple_fd, sizeof(t), t.record_id);
    return t;
}
//*********************************************************************************************************************
linda::TupleFileUtils::tuple linda::LindaCommunicator::readWhenNobodyFound(ProcessFileUtils::process &proc)
{
    std::cerr<<"NOBODY" << std::endl;
    ProcessFileUtils::unlockRecord(proc_fd, sizeof(proc), proc.record_id);
    linda::sigusr1Suspend();

    int new_fd = open((DEFAULT_FILEPATH + DEF_MES_FILE_PREF + std::to_string(proc.pid)).c_str(), O_RDWR, S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH);
    if(new_fd == -1)
        throw linda::LindaException("Unable to open auxiliary fileeee");

    ProcessFileUtils::lockRecord(proc_fd, sizeof(proc), proc.record_id);
    ProcessFileUtils::setRecordTaken(proc_fd, proc.record_id, 0);
    ProcessFileUtils::unlockRecord(proc_fd, sizeof(proc), proc.record_id);

    TupleFileUtils::tuple mes_t;
    TupleFileUtils::readRecord(new_fd, &mes_t, 0);

    return mes_t;
}
//*********************************************************************************************************************
void linda::ProcessFileUtils::process::initProcess(const std::string &pattern_, int input, int rec_id)
{
    timestamp = get_current_time();
    flag = input;
    found = 0;
    pid = getpid();
    if(pid == -1)
        throw linda::LindaException("Getpid failed");
    record_id = rec_id;
    taken = 1;
    pattern_.copy(pattern, pattern_.size());
    pattern[pattern_.size()] = '\0';
    std::cout << pattern << std::endl;
}
//*********************************************************************************************************************

