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
    tuple_fd = open((DEFAULT_FILEPATH+tuple_file).c_str(), O_RDWR|O_CREAT);
    proc_fd = open((DEFAULT_FILEPATH+process_file).c_str(), O_RDWR|O_CREAT);
    //blokowanie sygnałów
    std::cout << getpid() << std::endl;
    sigset_t block_set;

    if(sigemptyset(&block_set) == -1)
        throw linda::LindaException("Failed to empty set");

    if(sigaddset(&block_set, SIGINT) == -1)
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

void linda::LindaCommunicator::sortQueue(std::vector<ProcessFileUtils::process *> &queue)
{
    std::sort(queue.begin(), queue.end(), linda::compProc);

}

void linda::LindaCommunicator::wakeProcesses(int fd, linda::TupleFileUtils::tuple *tuple)
{
    std::vector<linda::ProcessFileUtils::process *> pids = linda::MatchesFinder::returnProcessQueue(tuple);
    sortQueue(pids);
    bool input = false;
    for (ProcessFileUtils::process *pid: pids)
    {
        if (!input)
        {

            ProcessFileUtils::process *ptr;
            ProcessFileUtils::readRecord(fd, ptr, pid->record_id);
            ptr->found = 1;
            ProcessFileUtils::writeRecord(fd, ptr, pid->record_id);
            ProcessFileUtils::unlockRecord(fd, sizeof(*ptr), ptr->record_id);
            int temp_fd = open((DEF_MES_FILE_PREF + std::to_string(pid->pid)).c_str(), O_RDWR | O_CREAT);
            TupleFileUtils::writeRecord(temp_fd, tuple, tuple->record_id);
            if(close(temp_fd)==-1)
                throw linda::LindaException("");
            wakeProcess(pid->pid);
            input = ptr->flag ? true : input;

        }
        else
        {
            ProcessFileUtils::unlockRecord(fd, sizeof(*pid), pid->record_id);
        }

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
    int rec_id = TupleFileUtils::findAndLock(tuple_fd);
    TupleFileUtils::tuple t;
    t.record_id = rec_id;
    strcpy(t.pattern, tuple.c_str());
    t.taken = true;

    TupleFileUtils::writeRecord(tuple_fd, &t, rec_id);

    wakeProcesses(proc_fd, &t);


}

linda::TupleFileUtils::tuple linda::LindaCommunicator::read_(std::string pattern, bool input)
{
    int rec_id = ProcessFileUtils::findAndLock(proc_fd); //wolne miejsce w pliku z procesami zakładamy że blokure obszar
    linda::ProcessFileUtils::process proc;
    proc.initProcess(pattern, input, rec_id); // towrzymy strukture
    ProcessFileUtils::writeRecord(proc_fd, &proc, proc.record_id); //wpisuje w obszar
    ProcessFileUtils::unlockRecord(proc_fd, sizeof(proc), proc.record_id); //zwlania
    TupleFileUtils::tuple *t = MatchesFinder::returnBlockedTuple(&proc); // znajduje krotke dla wybranego procesu
    ProcessFileUtils::lockRecord(proc_fd, sizeof(proc), proc.record_id); // blokuje obszar
    if (t) // krotka znaleziona
    {
        if (proc.found) //inny proces znalazł juz krotke dla naszego procesu
        {
            proc.taken = false;
            ProcessFileUtils::unlockRecord(proc_fd, sizeof(proc), proc.record_id);
            TupleFileUtils::unlockRecord(tuple_fd, sizeof(*t), t->record_id);
            linda::ExecuteThisThingThatWillSuspendProcesUntilItGetsSIGUSR1Signal();
        }
        else // nie znaleziono dla nas krotki, nasza jest tą jedyną
        {
            if (proc.flag) // nikt już nam jej nie zabierze
            {
                t->taken = false;
                TupleFileUtils::writeRecord(tuple_fd, t, t->record_id);
            }
            TupleFileUtils::unlockRecord(tuple_fd, sizeof(*t), t->record_id);
            ProcessFileUtils::unlockRecord(proc_fd, sizeof(proc), proc.record_id);
            return *t;
        }
    }
    else
    {
        ProcessFileUtils::unlockRecord(proc_fd, sizeof(proc), proc.record_id);
        linda::ExecuteThisThingThatWillSuspendProcesUntilItGetsSIGUSR1Signal();
    }
    int new_fd = open((DEF_MES_FILE_PREF + std::to_string(proc.pid)).c_str(), O_RDWR | O_CREAT);
    if(new_fd == -1)
        throw linda::LindaException("");
    TupleFileUtils::tuple *mes_t;
    TupleFileUtils::readRecord(new_fd, mes_t, 0);

    return *mes_t;
}

void linda::ProcessFileUtils::process::initProcess(const string &pattern, bool input, int rec_id)
{
    linda::ProcessFileUtils::process proc;
    proc.timestamp = get_current_time();
    proc.flag = input;
    proc.found = false;
    proc.pid = getpid();
    if(proc.pid == -1)
        throw linda::LindaException("");
    proc.record_id = rec_id;
    proc.taken = true;
    strcpy(proc.pattern, pattern.c_str());
}


