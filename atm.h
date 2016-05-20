#ifndef ATM_H
#define ATM_H

#include "account.h"
#include "banking.h"

typedef struct ATM_arg
{
    int id;
    char* log;
}ATM_args;

extern pthread_mutex_t create_acc;
extern int ATM_count;

void* ATM(void *arg);

#endif
