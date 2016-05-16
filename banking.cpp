#include "banking.h"

/* Function pointer class for using find_if() func */
class comare_for_find
{
    public:
        comare_for_find (Account* compare_to) : compare_to_(compare_to) { }
        bool operator()(Account *c1) const {return  *c1 == *compare_to_;}  

    private:
        const Account* compare_to_;
};

class compare_for_sort
{
    public:
        bool operator()(Account *acc1, Account *acc2) const {return  *acc1 < *acc2;}  
};

/* Creates new account and updates the Container of accounts
 * Param 1 - account id to be created
 * Param 2 - password for that account
 * Param 3 - amount to initialize account with */
void Create_acc(int acc_id, std::string pass, int amount)
{
    // assuming acc_id isnt related to existing account
    Account* new_acc = new Account(acc_id, amount, pass);
    b_accs.push_back(new_acc);
    sorted_accs.push_back(new_acc);
    std::sort(sorted_accs.begin(), sorted_accs.end(), compare_for_sort());
}

/* Searches the accounts container for the related acount per id
 * Param 1 - account id to be searched
 * Returns - pointer to the relevant account
 *  if not found return b_accs.end() */
Account* find_acc(int acc_id)
{
    Account to_find(acc_id);
    comare_for_find eq(&to_find);
    return *(std::find_if(b_accs.begin(), b_accs.end(), eq));
}

