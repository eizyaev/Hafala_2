#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "ATM.h"

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
        usleep(3);
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
        sleep(1);
        printf("ERROR %d: Unknown Command\n", ATM_id);
        return 1; 
    }
    args[0] = cmd;
    for (int i=1; i<MAX_ARG; i++)
        args[i] = strtok(NULL, delimiters); 

    int id = atoi(args[1]);
    std::string pass(args[2]);
    double new_balance = 0;
    Account* src = find_acc(id);

    // Open new account
    /**********************************************************************************************/    
    if (!strcmp(cmd, "O"))
    {
        double money = atoi(args[3]);
        if (src != NULL)
        {
            sleep(1);
            printf("Error %d: Your transaction failed – account id %d already exists\n", ATM_id, id);
            return 1;
        }
        sleep(1);
        Create_acc(id, pass, money);
        printf("%d: New account id is %d with password %s and initial balance %f\n", id, pass, money);
        return 0;
    }
    /**********************************************************************************************/    
    // General cases:
    // source account doesnt exist
    if (src == NULL)
    {
        sleep(1);
        printf("Error %d: Your transaction failed – account id %d does not exist\n", ATM_id, id);
        return 1;
    }
    // source account password incorrect
    if (!src->is_valid(pass))
    {
        sleep(1);
        printf("Error %d: Your transaction failed – password for account id %d is incorrect\n", ATM_id, id);
        return 1;
    }
    /**********************************************************************************************/    
    // Diposit money
    if(!strcmp(cmd, "D"))
    {
        double money = atoi(args[3]);
        new_balance = src->deposit(money);
        printf("%d: Account %d new balance is %f after %f $ was deposited\n",
                ATM_id, id, new_balance, money);
        return 0;
    }
    /**********************************************************************************************/    
    // Widthrew money
    else if(!strcmp(cmd, "W"))
    {
        double money = atoi(args[3]);
        if (!src->pull(money, &new_balance))
        {
            printf("Error %d: Your transaction failed – account id %d balance is lower than %f\n",
                    ATM_id, id, money);
            return 1;
        }
        else
        {
            printf("%d: Account %d new balance is %f after %f $ was withdrew\n",
                    ATM_id, id, new_balance, money);
            return 0;
        }

    }
    /**********************************************************************************************/    
    // Check balance
    else if(!strcmp(cmd, "B"))
    {
        new_balance = src->get_balance();
        printf("%d: Account %d balance is %f\n", ATM_id, id, new_balance);
    }
    /**********************************************************************************************/    
    // Transfer money
    else if(!strcmp(cmd, "T"))
    {
        int dst_id = atoi(args[3]);
        double money = atoi(args[4]);
        double dst_bal = 0;
        Account* dst_acc = find_acc(dst_id);
        if (dst_acc == NULL)
        {
            sleep(1);
            printf("Error %d: Your transaction failed – account id %d does not exist\n", ATM_id, dst_id);
            return 1;
        }
        if (!src->transfer(money, dst_acc, &new_balance, &dst_bal))
        {
            printf("Error %d: Your transaction failed – account id %d balance is lower than %f\n",
                    ATM_id, id, money);
            return 1;
        }
        printf("%d: Transfer %f from account %d to account %d new account"
                "balance is %f new target account balance is %f\n",
                ATM_id, money, id, dst_id, new_balance, dst_bal);
        return 0;
    }
    /**********************************************************************************************/    
    // Unknown operation
    else
    {
        printf("ERROR %d: Unknown Command\n", ATM_id);
        return 1;
    }
    /**********************************************************************************************/    
}
