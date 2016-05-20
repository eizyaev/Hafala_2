#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "atm.h"

#define MAX_ARG 5


int do_command(char* line, int ATM_id);
pthread_mutex_t create_acc;

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
        usleep(100);
        do_command(line, cur_args->id);
    }

    fclose(ATM_log);
    if (line)
        free(line);
    
    pthread_exit(NULL);
    return NULL;
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
    int pass = atoi(args[2]);
    double new_balance = 0;

    // Open new account
    /**********************************************************************************************/    
    if (!strcmp(cmd, "O"))
    {
        pthread_mutex_lock(&create_acc);

        Account* src = find_acc(id);
        double money = atoi(args[3]);
        if (src != NULL)
        {
            sleep(1);
            printf("Error %d: Your transaction failed – account id %d already exists\n", ATM_id, id);
            pthread_mutex_unlock(&create_acc);
            return 1;
        }
        sleep(1);
        Create_acc(id, pass, money);

        pthread_mutex_unlock(&create_acc);

        printf("%d: New account id is %d with password %d and initial balance %.0f\n", ATM_id, id, pass, money);
        return 0;
    }
    /**********************************************************************************************/    
    // General cases:
    // source account doesnt exist
    Account* src = find_acc(id);
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
        printf("%d: Account %d new balance is %.0f after %.0f $ was deposited\n",
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
            printf("Error %d: Your transaction failed – account id %d balance is lower than %.0f\n",
                    ATM_id, id, money);
            return 1;
        }
        else
        {
            printf("%d: Account %d new balance is %.0f after %.0f $ was withdrew\n",
                    ATM_id, id, new_balance, money);
            return 0;
        }

    }
    /**********************************************************************************************/    
    // Check balance
    else if(!strcmp(cmd, "B"))
    {
        new_balance = src->get_balance();
        printf("%d: Account %d balance is %.0f\n", ATM_id, id, new_balance);
        return 0;
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
            printf("Error %d: Your transaction failed – account id %d balance is lower than %.0f\n",
                    ATM_id, id, money);
            return 1;
        }
        printf("%d: Transfer %.0f from account %d to account %d new account"
                "balance is %.0f new target account balance is %.0f\n",
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
