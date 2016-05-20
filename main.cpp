#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "atm.h"


int main(int argc, char *argv[])
{
    if (argc < 3)
    {
        printf("Incorrect imput, Exiting...\n");
        return 1;
    }

    int i=0; 
    int atm_num = atoi(argv[1]);
    ATM_args* atm = (ATM_args*)malloc(sizeof (ATM_args)*atm_num);
    pthread_t *atm_threads= (pthread_t*)malloc(sizeof(pthread_t)*atm_num);
    pthread_mutex_init(&create_acc, NULL);

    for (i=0 ; i < atm_num ; i++)
    {
        atm[i].log = (char*)malloc(strlen(argv[i+2])+1);
        strcpy(atm[i].log, argv[i+2]);
        atm[i].id = i+1;
        pthread_create(&atm_threads[i], NULL, ATM, (void*)&atm[i]);
    }

    for(i=0; i < atm_num; i++)
        pthread_join(atm_threads[i], NULL);

    pthread_mutex_destroy(&create_acc);

    
    // TODO: add memory free
    return 0;
}
