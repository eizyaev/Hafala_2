#include "banking.h"

/* The main database for the bank, container of accounts
 * b_accs - used for reading/writing
 * sorted_accs - used for printing bank status in sorted order */
std::vector<Account*> b_accs, sorted_accs;

/* Function pointer class for using std::find_if() func */
class comare_for_find
{
    public:
        comare_for_find (Account* compare_to) : compare_to_(compare_to) { } // constructor
        bool operator()(Account *c1) const {return  *c1 == *compare_to_;} // operator() overload 

    private:
        const Account* compare_to_; // account to be searched by find_if()
};

/* Function pointer class for using std::sort() func */
class compare_for_sort
{
    public:
        // operator() overload with 2 arguments for sort()
        bool operator()(Account *acc1, Account *acc2) const {return  *acc1 < *acc2;}  
};

/* Creates new account and updates the Container of accounts
 * Param 1 - account id to be created
 * Param 2 - password for that account
 * Param 3 - amount to initialize account with */
void Create_acc(int acc_id, int pass, double amount)
{
    Account* new_acc = new Account(acc_id, amount, pass); // new account
    b_accs.push_back(new_acc); // inserting into our bank
    sorted_accs.push_back(new_acc); // inserting into duplicate vector which is always sorted
    std::sort(sorted_accs.begin(), sorted_accs.end(), compare_for_sort()); // sorting by id
}

/* Searches the accounts container for the related acount per id
 * Param 1 - account id to be searched
 * Returns - pointer to the relevant account
 *  if not found return b_accs.end() */
Account* find_acc(int acc_id)
{
    Account to_find(acc_id); // demo account to be searched by id
    comare_for_find eq(&to_find); // building the compare object
    std::vector<Account*>::iterator it = (std::find_if(b_accs.begin(), b_accs.end(), eq));
    if (it == b_accs.end())
        return NULL;
    else
        return *(it);
}

