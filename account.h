#ifndef ACCOUNT_H
#define ACCOUNT_H

#include <pthread.h>
#include <string>

class Account
{
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

    public:
        Account(int id = 0, double money = 0, const std::string& password = ""); // TODO include default cons for find()
        ~Account();
        void deposit(double money);
        double get_balance();
        bool is_valid(std::string password);
        bool operator<(const Account& acc) const; // TODO correct syntax?
        bool operator==(const Account& acc) const; // TODO correct syntax?
};

#endif
