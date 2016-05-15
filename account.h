#ifndef ACCOUNT_H
#define ACCOUNT_H

#include <pthread.h>

class Account
{
    public:
        Account(int id);
        Account(int id , double money, std::string password); // TODO include default cons for find()
        void deposit(std::string password, double money);
        void pull(std::string password, double money);
        double balance(std::string password);

        bool operator<(const Account& acc) const; // TODO correct syntax?
        bool operator==(const Account& acc) const; // TODO correct syntax?

    private:
        // account data
        int acc_id;
        double balance;
        const std::string pass;

        // counters for the readers/writers implementation
        int readcount;
        int writecount;

        // Mutex's for the readers/writers implementation
        pthread_mutex_t r_mutex;
        pthread_mutex_t w_mutex;
        pthread_mutex_t block_r_mutex;
        pthread_mutex_t resource;

};

#endif
