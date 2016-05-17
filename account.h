#ifndef ACCOUNT_H
#define ACCOUNT_H

#include <pthread.h>
#include <string>

/* Class for thread safe account managment */
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
        Account(int id = 0, double money = 0, const std::string& password = ""); // constructor
        ~Account(); // distructor

        /* depositis money into the account */
        void deposit(double money); 
        /* pulls money from the account, returns false if there's 
         * not enough money to be pulled */
        bool pull(double money);
        /* retunrs the account balance */
        double get_balance(); 
        /* checks if the entered password is correct */
        bool is_valid(std::string password); 
         /* Transfer money from this account to dest account*/
        bool transfer(double money, Account* dest);

        bool operator<(const Account& acc) const; // operator overload for sorting accounts by id
        bool operator==(const Account& acc) const; // operator overload for searching account by its id
};

#endif
