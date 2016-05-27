#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include "atm.h"

int main(int argc, char *argv[])
{
    // minimum arguments check
    if (argc < 3)
    {
        printf("illegal arguments\n");
        return 1;
    }

    int i=0; 
    int atm_num = atoi(argv[1]);
    ATM_count = atm_num;
    // atm's args allocating space
    ATM_args* atm = (ATM_args*)malloc(sizeof (ATM_args)*atm_num);
    if (atm == NULL)
    {
        fprintf(stderr, "Error not enough space\n");
        exit(1);
    }

    // allocation atm's ptreads
    pthread_t *atm_threads= (pthread_t*)malloc(sizeof(pthread_t)*atm_num);
    if (atm == NULL)
    {
        fprintf(stderr, "Error not enough space\n");
        free(atm);
        exit(1);
    }

    // bank threads
    pthread_t bank_print, commission_collector;
    if (pthread_create(&bank_print, NULL, print_status, (void*)NULL))
    {
        fprintf(stderr, "Failed to create: bank printing thread\n");
        exit(1);
    }
    if (pthread_create(&commission_collector, NULL, commission, (void*)NULL))
    {
        fprintf(stderr, "Failed to create: commission collector thread\n");
        exit(1);
    }

    // database readers/writers mutexes
    if (pthread_mutex_init(&create_mutex, NULL))
    {
        fprintf(stderr, "Failed to init mutex\n");
        exit(1);
    }
    if (pthread_mutex_init(&find_mutex, NULL))
    {
        fprintf(stderr, "Failed to init mutex\n");
        exit(1);
    }
    if (pthread_mutex_init(&resource_mutex, NULL))
    {
        fprintf(stderr, "Failed to init mutex\n");
        exit(1);
    }
    if (pthread_mutex_init(&block_mutex, NULL))
    {
        fprintf(stderr, "Failed to init mutex\n");
        exit(1);
    }
    
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
            for (int j=0 ; j <= i ; j++)
                free(atm[j].log);
            free(atm_threads);
            free(atm);
            fprintf(stderr, "Error not enough space\n");
            exit(1);
        }
        strcpy(atm[i].log, argv[i+2]);
        atm[i].id = i+1;
        // creating atm's threads
        if (pthread_create(&atm_threads[i], NULL, ATM, (void*)&atm[i]))
        {
            for (int j=0 ; j <= i ; j++)
                free(atm[j].log);
            free(atm_threads);
            free(atm);
            fprintf(stderr, "Error not enough space\n");
            exit(1);
        }
    }

    // collecting finshed atm threads
    for(i=0; i < atm_num; i++)
        if (pthread_join(atm_threads[i], NULL))
        {
            fprintf(stderr, "Failed to join: ATM %d thread\n", i+1);
            exit(1);
        }
    // collecting banking printing thread
    if (pthread_join(bank_print, NULL))
        {
            fprintf(stderr, "Failed to join: bank printing thread\n");
            exit(1);
        }
    if (pthread_join(commission_collector, NULL))
        {
            fprintf(stderr, "Failed to join: commission collector thread\n");
            exit(1);
        }

    // destroying database mutexes
    if (pthread_mutex_destroy(&create_mutex))
    {
        fprintf(stderr, "Failed to destroy mutex\n");
        exit(1);
    }
    if (pthread_mutex_destroy(&find_mutex))
    {
        fprintf(stderr, "Failed to destroy mutex\n");
        exit(1);
    }
    if (pthread_mutex_destroy(&resource_mutex))
    {
        fprintf(stderr, "Failed to destroy mutex\n");
        exit(1);
    }
    if (pthread_mutex_destroy(&block_mutex))
    {
        fprintf(stderr, "Failed to destroy mutex\n");
        exit(1);
    }

    // closing log.txt file
    fclose(f);
    if (f)
        free(f);

    // memory free
    for (i=0 ; i < atm_num ; i++)
        free(atm[i].log);
    free(atm_threads);
    free(atm);

    // accounts memory free
    for(std::vector<Account*>::iterator it = b_accs.begin(); it != b_accs.end(); ++it)
        delete (*it);

    return 0;
}
