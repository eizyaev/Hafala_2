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

    printf("number of arguments is %d\n", argc);
    printf("the size of ATM_args is: %lu\n", sizeof(ATM_args));
    printf("the size of \"hello world\" is: %lu\n", sizeof("hello world"));

    int i=0; 
    int atm_num = atoi(argv[1]);
    ATM_args* atm = (ATM_args*)malloc(sizeof (ATM_args)*atm_num);
    pthread_t *atm_threads= (pthread_t*)malloc(sizeof(pthread_t)*atm_num);

    for (i=0 ; i < atm_num ; i++)
    {
        printf("the size of argument %d is : %lu\n", i+2, strlen(argv[i+2])+1);
        atm[i].log = (char*)malloc(strlen(argv[i+2])+1);
        strcpy(atm[i].log, argv[i+2]);
        atm[i].id = i+1;
    }

    for (i=0 ; i < atm_num ; i++)
        printf("Name of ATM %d: %s\n", atm[i].id, atm[i].log);

    for(i=0; i < atm_num; i++)
    {
        pthread_create(&atm_threads[i], NULL, ATM, (void*)&atm[i]);
    }

    for(i=0; i < atm_num; i++)
        pthread_join(atm_threads[i], NULL);


    return 0;
}
