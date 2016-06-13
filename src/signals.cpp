//
// Created by tomasz on 13.06.16.
//

#include <iostream>
#include <signal.h>
#include <linda_exception.h>


void ExecuteThisThingThatWillSuspendProcesUntilItGetsSIGUSR1Signal()
{
    sigset_t suspend_set;
    const int i = sigfillset(&suspend_set);
    if(i == -1)
        throw linda::LindaException("");
    const int i1 = sigdelset(&suspend_set, SIGUSR1);
    if(i1 == -1)
        throw linda::LindaException("");
    const int i2 = sigsuspend(&suspend_set);
    if(i2==-1)
        throw linda::LindaException("");
}