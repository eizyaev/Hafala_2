#ifndef BANKING_H
#define BANKING_H

#include <algorithm>
#include <vector>
#include "account.h"

/* The main database for the bank, container of accounts
 * b_accs - used for reading/writing */
extern std::vector<Account*> b_accs;
extern double bank_account;

/* The bank status printing thread function
 * Prints the status of all accounts every 0.5 second */
void* print_status(void *arg);
/* Bank's commission collecting threads*/
void* commission(void *arg);
void Create_acc(int acc_id, int pass, double amount); // creates new account
Account* find_acc(int acc_id); // searches account by its id number
Account* find_acc_for_create(int acc_id); // lock free search

#endif
