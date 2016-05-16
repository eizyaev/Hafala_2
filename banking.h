#ifndef BANKING_H
#define BANKING_H

#include <algorithm>
#include <vector>
#include "account.h"

std::vector<Account*> b_accs, sorted_accs;

void Create_acc(int acc_id, std::string pass, int amount);
Account* find_acc(int acc_id);

#endif
