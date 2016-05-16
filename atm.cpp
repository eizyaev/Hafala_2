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

int do_command(char* line, int ATM_id);

void* ATM(void *arg)
{
    ATM_args* cur_args = (ATM_args*)arg; 

    // openning log file
    FILE *ATM_log = fopen(cur_args->log, "r");
    if (ATM_log == 0) {
        fprintf(stderr, "cannot open log file\n");
        exit(1);
    }

    // variables for using getline()
    char * line = NULL;
    size_t len = 0;
    ssize_t read;

    while ((read = getline(&line, &len, ATM_log)) != -1)
    {
        do_command(line, cur_args->id);
    }

    fclose(ATM_log);
    if (line)
        free(line);
    exit(0);

} 

int do_command(char* line, int ATM_id)
{

    char const* delimiters = " \t\n";  
    char* cmd;
    char* args[MAX_ARG];

    // parsing the line into the command and its arguments
    cmd = strtok(line, delimiters);
    if (cmd == NULL)
    {
        printf("ERROR %d: Unknown Command\n", ATM_id);
        return 1; 
    }
    args[0] = cmd;
    for (int i=1; i<MAX_ARG; i++)
        args[i] = strtok(NULL, delimiters); 

    // Open new account
    if (!strcmp(cmd, "O"))
    {
        int id = atoi(args[1]);
        std::string pass(args[2]);
        double money = atoi(args[3]);

        if (find_acc(id) == NULL)
        {
            printf("Error %d: Your transaction failed – account with the same id exists\n", ATM_id);
            return 1;
        }
        
        Create_acc(id, pass, money);
    }
    
    // Diposit money
    else if(!strcmp(cmd, "D"))
    {
    }

    // Widthraw money
    else if(!strcmp(cmd, "W"))
    {
    }

    // Check balance
    else if(!strcmp(cmd, "B"))
    {
    }

    // Transfer money
    else if(!strcmp(cmd, "T"))
    {
    }
    // Unknown operation
    else
        printf("ERROR %d: Unknown Command\n", ATM_id);

}
