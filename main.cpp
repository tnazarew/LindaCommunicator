#include <iostream>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <PatternComparator.h>
#include <ProccessFileUtils.h>
#include <TupleFileUtils.h>
#include <MatchesFinder.h>

using namespace linda;

int main()
{
    int lfd = open("/tmp/linda_tuples.txt", O_RDWR | O_CREAT);
    int pfd = open("/tmp/linda_proc.txt", O_RDWR | O_CREAT);
    ProccessFileUtils::process pr;
    TupleFileUtils::tuple tu;
    std::string tuple_pattern = "1,\"abc\",3.1415,\"e\"";
    std::string process_pattern = "i:1,s:\"abc\",f:*,s:>\"d\"";

    ProccessFileUtils p;
    TupleFileUtils t;

    strcpy(pr.pattern, process_pattern.c_str());
    pr.pid = 1;
    pr.flag = 1;
    pr.taken = 1;
    pr.timestamp = time(NULL);
    for(int i = 0; i < 10; ++i) {
        p.lockRecord(pfd, sizeof(struct ProccessFileUtils::process), i);
        p.writeRecord(pfd, &pr, i);
        p.unlockRecord(pfd, sizeof(struct ProccessFileUtils::process), i);
    }

    strcpy(tu.pattern, tuple_pattern.c_str());
    tu.taken = 1;

    for(int i = 0; i < 10; ++i) {
        t.lockRecord(lfd, sizeof(struct TupleFileUtils::tuple), i);
        t.writeRecord(lfd, &tu, i);
        t.unlockRecord(lfd, sizeof(struct TupleFileUtils::tuple), i);
    }

    TupleFileUtils::tuple* foundTuple = MatchesFinder::returnBlockedTuple(&pr);


    std::vector<ProccessFileUtils::process *> processes = MatchesFinder::returnProcessQueue(foundTuple);

    int fd = open("filename", O_RDWR|O_CREAT);
    TupleFileUtils::tuple tuple;

    close(fd);



    return 0;
}
