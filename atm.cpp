#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "account.h"
#include "banking.h"

#define MAX_ARG 5

typedef struct ATM_arg
{
    int id;
    char* log;
} ATM_args;


//void* ATM(void *arg)
int main(void)
{
//    ATM_args* cur_args = (ATM_args*)arg; 
    char log[] = "short_atm.txt";
    FILE *ATM_log = fopen(log, "r");
    if (ATM_log == 0) {
        fprintf(stderr, "cannot open log file\n");
        exit(1);
    }

    char * line = NULL;
    size_t len = 0;
    ssize_t read;

    while ((read = getline(&line, &len, ATM_log)) != -1)
    {
        printf("Retrieved line of length %zu :\n", read);
        printf("%s", line);

	char const* delimiters = " \t\n";  
    char* cmd;
	char* args[MAX_ARG];

    cmd = strtok(line, delimiters);
    if (cmd == NULL)
    {
        printf("Incorrect input\n");
        continue; 
    }
    args[0] = cmd;
	for (int i=1; i<MAX_ARG; i++)
		args[i] = strtok(NULL, delimiters); 

    printf("The arguments are: %s %s %s %s %s\n\n", args[0], args[1], args[2], args[3], args[4]);




    }
    fclose(ATM_log);
    if (line)
        free(line);
    exit(0);

    //int i;
    //for (i=0; i<TCOUNT; i++) {
        //pthread_mutex_lock(&count_mutex);
        //count++;
        //if (count == COUNT_LIMIT) {
            //pthread_cond_signal(&count_threshold_cv);
        //}
        //pthread_mutex_unlock(&count_mutex);
    //}
    //pthread_exit(NULL);
} 
