#ifndef ATM_H
#define ATM_H

#include "account.h"
#include "banking.h"

typedef struct ATM_arg
{
    int id;
    char* log;
}ATM_args;

void* ATM(void *arg);

#endif
