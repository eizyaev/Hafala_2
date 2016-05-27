#ifndef ACCOUNT_H
#define ACCOUNT_H

#include <pthread.h>

/* Class for thread safe account managment */
class Account
{
    private:
        // account data
        int acc_id;
        double balance;
        const int pass;

        // counters for the readers/writers implementation
        int readcount;
        int writecount;

        // Mutex's for the readers/writers implementation
        pthread_mutex_t r_mutex;
        pthread_mutex_t w_mutex;
        pthread_mutex_t block_r_mutex;
        pthread_mutex_t resource;

    public:
        Account(int id = 0, double money = 0, const int password = 0); // constructor
        ~Account(); // distructor

        /* depositis money into the account */
        double deposit(double money); 
        /* pulls money from the account, returns false if there's 
         * not enough money to be pulled */
        bool pull(double money, double* new_bal);
        /* retunrs the account balance */
        double get_balance(); 
        /* retunrs the account balance without delay */
        double bank_get_balance();
        /* get rounded %com money from this account */
        double get_commission(double com);
        // get account's id
        int get_id();
        // get account's password
        int get_pass();
        /* checks if the entered password is correct */
        bool is_valid(int password); 
         /* Transfer money from this account to dest account*/
        void transfer(double money, Account* dest, double *src_bal, double* dst_bal);

        bool operator<(const Account& acc) const; // operator overload for sorting accounts by id
        bool operator==(const Account& acc) const; // operator overload for searching account by its id
};

#endif
