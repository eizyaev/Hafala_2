#ifndef ATM_H
#define ATM_H

#include <unistd.h>
#include <stdio.h>
#include "account.h"
#include "banking.h"

// argument passed to each ATM
typedef struct ATM_arg
{
    int id;
    char* log;
}ATM_args;

// database mutexes
extern pthread_mutex_t create_mutex, find_mutex, resource_mutex, block_mutex;
// ATM active counter
extern int ATM_count;
extern int create_counter;
extern int find_counter;
// log.txt output file
extern FILE *f;

/* ATM thread function */
void* ATM(void *arg);

#endif
