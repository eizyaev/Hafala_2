#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include "atm.h"

int main(int argc, char *argv[])
{
    if (argc < 3)
    {
        printf("illegal arguments\n");
        return 1;
    }

    int i=0; 
    int atm_num = atoi(argv[1]);
    ATM_count = atm_num;
    ATM_args* atm = (ATM_args*)malloc(sizeof (ATM_args)*atm_num);
    if (atm == NULL)
    {
        fprintf(stderr, "Error not enough space\n");
        exit(1);
    }

    pthread_t *atm_threads= (pthread_t*)malloc(sizeof(pthread_t)*atm_num);
    if (atm == NULL)
    {
        fprintf(stderr, "Error not enough space\n");
        free(atm);
        exit(1);
    }
    // bank printing thread
    pthread_t bank_print;
    pthread_create(&bank_print, NULL, print_status, (void*)NULL); // TODO syscall

    pthread_mutex_init(&create_mutex, NULL); // TODO syscall
    pthread_mutex_init(&find_mutex, NULL); // TODO syscall
    pthread_mutex_init(&resource_mutex, NULL); // TODO syscall
    pthread_mutex_init(&block_mutex, NULL); // TODO syscall
    
    // openning log.txt file for write
    f = fopen("log.txt", "w");
    if (f == NULL)
    {
        fprintf(stderr, "Error opening file!\n");
        exit(1);
    }

    for (i=0 ; i < atm_num ; i++)
    {
        atm[i].log = (char*)malloc(strlen(argv[i+2])+1);
        if (atm[i].log == NULL)
        {
            for (i=0 ; i <= atm_num ; i++)
                free(atm[i].log); // TODO syscall
            free(atm_threads);
            free(atm);
            fprintf(stderr, "Error not enough space\n");
            exit(1);
        }
        strcpy(atm[i].log, argv[i+2]);
        atm[i].id = i+1;
        pthread_create(&atm_threads[i], NULL, ATM, (void*)&atm[i]); // TODO syscall
    }

    for(i=0; i < atm_num; i++)
        pthread_join(atm_threads[i], NULL); // TODO syscall
    pthread_join(bank_print, NULL); // TODO syscall

    pthread_mutex_destroy(&create_mutex); // TODO syscall
    pthread_mutex_destroy(&find_mutex); // TODO syscall
    pthread_mutex_destroy(&resource_mutex); // TODO syscall
    pthread_mutex_destroy(&block_mutex); // TODO syscall

    // closing log.txt file
    fclose(f); // TODO syscall
    if (f)
        free(f);

    // memory free
    for (i=0 ; i < atm_num ; i++)
        free(atm[i].log); // TODO syscall
    free(atm_threads);
    free(atm);
    
    for(std::vector<Account*>::iterator it = b_accs.begin(); it != b_accs.end(); ++it)
        delete (*it);

    printf("FINISHED\n");

    return 0;
}
