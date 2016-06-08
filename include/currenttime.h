#include <sys/time.h>
using namespace std;

long get_current_time()
{
    struct timeval tv;

    gettimeofday(&tv, NULL);

    unsigned long long milliseconds_since_epoch =
        (unsigned long long)(tv.tv_sec) * 1000 +
        (unsigned long long)(tv.tv_usec) / 1000;

    return milliseconds_since_epoch;
}

