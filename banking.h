#ifndef BANKING_H
#define BANKING_H

#include <algorithm>
#include <vector>
#include "account.h"

/* The main database for the bank, container of accounts
 * b_accs - used for reading/writing
 * sorted_accs - used for printing bank status in sorted order */
extern std::vector<Account*> b_accs, sorted_accs;

void Create_acc(int acc_id, int pass, double amount); // creates new account
Account* find_acc(int acc_id); // searches account by its id number

#endif
