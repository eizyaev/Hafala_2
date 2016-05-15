#include "account.h"

Account(int id):
        acc_id(id), balance(0), pass("") { }
        

Account(int id, double money, const std::string& password):
        acc_id(id), balance(money), pass(password)
{
    pthread_mutex_init(&r_mutex, NULL); 
    pthread_mutex_init(&w_mutex, NULL); 
    pthread_mutex_init(&block_r_mutex, NULL); 
    pthread_mutex_init(&resource, NULL); 
}


void deposit(std::string password, double money);
void pull(std::string password, double money);
double balance(std::string password);

bool operator < (const Account& acc) const; // TODO correct syntax?

