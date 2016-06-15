//
// Created by tomasz on 13.06.16.
//

#include <iostream>
#include <signal.h>
#include <signals.h>
#include <linda_exception.h>
#include <cstring>
//*********************************************************************************************************************
void linda::sigusr1Suspend()
{
    sigset_t suspend_set;
    const int i = sigfillset(&suspend_set);
    if(i == -1)
        throw linda::LindaException(strerror(errno));
    const int i1 = sigdelset(&suspend_set, SIGUSR1);
    if(i1 == -1)
        throw linda::LindaException(strerror(errno));
    sigsuspend(&suspend_set);
}
//*********************************************************************************************************************
