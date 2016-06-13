//
// Created by tomasz on 13.06.16.
//

#include <iostream>
#include <signal.h>


void ExecuteThisThingThatWillSuspendProcesUntilItGetsSIGUSR1Signal()
{
    sigset_t suspend_set;
    sigfillset(&suspend_set);
    sigdelset(&suspend_set, SIGUSR1);
    sigsuspend(&suspend_set);
}