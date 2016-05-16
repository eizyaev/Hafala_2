#include "account.h"

//READERS WRITERS 2016S

/* The constructor of the class */
Account::Account(int id, double money, const std::string& password):
acc_id(id), balance(money), pass(password), readcount(0), writecount(0)
{   /* Initializing all local account mutexes */
    pthread_mutex_init(&r_mutex, NULL);
    pthread_mutex_init(&w_mutex, NULL); 
    pthread_mutex_init(&block_r_mutex, NULL); 
    pthread_mutex_init(&resource, NULL); 
}

/* The destructor of the class */
Account::~Account()
{   /* destroying all local account mutexes */
    pthread_mutex_destroy(&r_mutex); 
    pthread_mutex_destroy(&w_mutex); 
    pthread_mutex_destroy(&block_r_mutex); 
    pthread_mutex_destroy(&resource); 
}
/* returns true if the password is correct */
bool Account::is_valid(std::string password)
{
    return pass == password;
}

/* Writers implemantion per account */
void Account::deposit(double money)
{
    pthread_mutex_lock(&w_mutex); //reserve entry section for writers - avoids race conditions
    writecount++; //report yourself as a writer entering
    if (writecount == 1) //checks if you're first writer
        pthread_mutex_lock(&block_r_mutex); //if you're first, then you must lock the readers out. Prevent them from trying to enter CS
    pthread_mutex_unlock(&w_mutex); //release entry section

    // writing is performed
    pthread_mutex_lock(&resource); //reserve the resource for yourself - prevents other writers from simultaneously editing the shared resource
    balance += money;  // writing is performed
    pthread_mutex_unlock(&resource); //release file


    pthread_mutex_lock(&w_mutex); //reserve exit section
    writecount--; //indicate you're leaving
    if (writecount == 0) //checks if you're the last writer
        pthread_mutex_unlock(&block_r_mutex); //if you're last writer, you must unlock the readers. Allows them to try enter CS for reading
    pthread_mutex_unlock(&w_mutex); //release exit section
}

/* Readers implemantion per account */
double Account::get_balance()
{
    pthread_mutex_lock(&block_r_mutex); //Indicate a reader is trying to enter

    pthread_mutex_lock(&r_mutex); //lock entry section to avoid race condition with other readers
    readcount++; //report yourself as a reader
    if (readcount == 1) //checks if you are first reader
        pthread_mutex_lock(&resource); //if you are first reader, lock the resource
    pthread_mutex_unlock(&r_mutex); //release entry section for other readers

    pthread_mutex_unlock(&block_r_mutex); //indicate you are done trying to access the resource

    // reading is performed
    double cur_balance = balance;

    pthread_mutex_lock(&r_mutex); //reserve exit section - avoids race condition with readers
    readcount--; //indicate you're leaving
    if (readcount == 0) //checks if you are last reader leaving
        pthread_mutex_unlock(&resource); //if last, you must release the locked resource
    pthread_mutex_unlock(&r_mutex); //release exit section for other readers

    return cur_balance;
}

/* Operator overload for comparing accounts by their id number */
bool Account::operator<(const Account& acc) const 
{
    return acc_id < acc.acc_id;
}

/* Operator overload for comparing accounts by their id number */
bool Account::operator==(const Account& acc) const 
{
    return acc_id == acc.acc_id;
}

