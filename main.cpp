#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include "atm.h"
FILE *f;

int main(int argc, char *argv[])
{
    if (argc < 3)
    {
        printf("illegal arguments\n");
        return 1;
    }

    int i=0; 
    int atm_num = atoi(argv[1]);
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

    pthread_mutex_init(&create_acc, NULL); // TODO syscall
    ATM_count = atm_num;
    printf("DEBUG : Number of ATM's %d\n", ATM_count); // TODO delete
    
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

    pthread_mutex_destroy(&create_acc); // TODO syscall

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

    return 0;
}
