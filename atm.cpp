#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "atm.h"

#define MAX_ARG 5


int do_command(char* line, int ATM_id);
// database multithreading mutexes
pthread_mutex_t create_mutex, find_mutex, resource_mutex, block_mutex;
// database readers/writers counters
int create_counter = 0;
int find_counter = 0;
// active ATM's counter
int ATM_count;
FILE *f;

/* ATM thread function */
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

    // read line from .txt file
    while ((read = getline(&line, &len, ATM_log)) != -1)
    {
        usleep(100);
        do_command(line, cur_args->id);
    }

    // closing opened file
    fclose(ATM_log);
    if (line)
        free(line);
    
    ATM_count--;
    pthread_exit(NULL);
    return NULL;
} 

/* perform ATM command
 * Param 1 - command line 
 * Param 2 - ATM's id numer 
 * Return 0/1 for success status*/
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
        fprintf(f, "ERROR %d: Unknown Command\n", ATM_id);
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
    //READERS WRITERS 2016S
    if (!strcmp(cmd, "O"))
    {
        int ret = 0;
        double money = atoi(args[3]);
        
        // entry section
        pthread_mutex_lock(&create_mutex); // lock entry
        create_counter++; //report yourself as a writer
        if (create_counter == 1) //checks if you're first writer
            pthread_mutex_lock(&block_mutex); //if you're first, lock the readers out 
        pthread_mutex_unlock(&create_mutex); //release entry section
        
        // writing section - creating new account
        pthread_mutex_lock(&resource_mutex); // lock the database

        Account* src = find_acc_for_create(id);
        if (src != NULL)
        {
            sleep(1);
            fprintf(f, "Error %d: Your transaction failed – account id %d already exists\n", ATM_id, id);
        }
        else
        {
            sleep(1);
            Create_acc(id, pass, money);
            fprintf(f ,"%d: New account id is %d with password %d and initial balance %.0f\n", ATM_id, id, pass, money);
        }
        pthread_mutex_unlock(&resource_mutex); // unlock the database

        // exit section
        pthread_mutex_lock(&create_mutex); //reserve exit section
        create_counter--; //indicate you're leaving
        if (create_counter == 0) //checks if you're the last writer
            pthread_mutex_unlock(&block_mutex); //if you're last writer, allow readers to enter
        pthread_mutex_unlock(&create_mutex); //release exit section

        return ret;
    }
    /**********************************************************************************************/    
    // General cases:
    // source account doesnt exist
    Account* src = find_acc(id);
    if (src == NULL)
    {
        sleep(1);
        fprintf(f, "Error %d: Your transaction failed – account id %d does not exist\n", ATM_id, id);
        return 1;
    }
    // source account password incorrect
    if (!src->is_valid(pass))
    {
        sleep(1);
        fprintf(f, "Error %d: Your transaction failed – password for account id %d is incorrect\n", ATM_id, id);
        return 1;
    }
    /**********************************************************************************************/    
    // Diposit money
    if(!strcmp(cmd, "D"))
    {
        double money = atoi(args[3]);
        new_balance = src->deposit(money);
        fprintf(f, "%d: Account %d new balance is %.0f after %.0f $ was deposited\n",
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
            fprintf(f, "Error %d: Your transaction failed – account id %d balance is lower than %.0f\n",
                    ATM_id, id, money);
            return 1;
        }
        else
        {
            fprintf(f, "%d: Account %d new balance is %.0f after %.0f $ was withdrew\n",
                    ATM_id, id, new_balance, money);
            return 0;
        }

    }
    /**********************************************************************************************/    
    // Check balance
    else if(!strcmp(cmd, "B"))
    {
        new_balance = src->get_balance();
        fprintf(f, "%d: Account %d balance is %.0f\n", ATM_id, id, new_balance);
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
            fprintf(f, "Error %d: Your transaction failed – account id %d does not exist\n", ATM_id, dst_id);
            return 1;
        }
        if (!src->transfer(money, dst_acc, &new_balance, &dst_bal))
        {
            fprintf(f, "Error %d: Your transaction failed – account id %d balance is lower than %.0f\n",
                    ATM_id, id, money);
            return 1;
        }
        fprintf(f, "%d: Transfer %.0f from account %d to account %d new account"
                "balance is %.0f new target account balance is %.0f\n",
                ATM_id, money, id, dst_id, new_balance, dst_bal);
        return 0;
    }
    /**********************************************************************************************/    
    // Unknown operation
    else
    {
        sleep(1);
        fprintf(f, "ERROR %d: Unknown Command\n", ATM_id);
        return 1;
    }
    /**********************************************************************************************/    
}
