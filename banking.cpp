#include "banking.h"
#include "atm.h"

/* The main database for the bank, container of accounts
 * b_accs - used for reading/writing
 * sorted_accs - used for printing bank status in sorted order */
std::vector<Account*> b_accs, sorted_accs;

/* The bank status printing thread function
 * Prints the status of all accounts every 0.5 second */
void* print_status(void *arg)
{
    printf("\033[2J"); // clear screen
    while(ATM_count > 0) // run as long there are active atm's
    {
        usleep(500);
        printf("\033[1;1H"); // move cursor to the beginning
        double balance;
        int id, pass;
        printf("Current Bank Status\n");

        // entry section
        pthread_mutex_lock(&block_mutex); // A reader is trying to enter

        pthread_mutex_lock(&find_mutex); // Lock entry
        find_counter++; //report yourself as a reader
        if (find_counter == 1) //checks if you're first reader
            pthread_mutex_lock(&resource_mutex); //if you're first, lock the database
        pthread_mutex_unlock(&find_mutex); //release entry section
        pthread_mutex_unlock(&block_mutex); // done trying access the database
        
        // database section reading, (iterating)
        for(std::vector<Account*>::iterator it = sorted_accs.begin(); it != sorted_accs.end(); ++it)
        {
            balance = (*it)->bank_get_balance();
            id = (*it)->get_id();
            pass = (*it)->get_pass();
            printf("Account %5d: Balance – %5.0f $ , Account Password – %5d\n", id, balance, pass); 
        }

        // exit section
        pthread_mutex_lock(&find_mutex); // Lock exit section
        find_counter--; // Indicate you're leaving
        if (find_counter == 0) // check if you're last 
            pthread_mutex_unlock(&resource_mutex); // release database if you're last  
        pthread_mutex_unlock(&find_mutex); //release exit section

    }
    pthread_exit(NULL);
    return NULL;
}

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
    Account* ret;

    // entry section
    pthread_mutex_lock(&block_mutex); // A reader is trying to enter

    pthread_mutex_lock(&find_mutex); // Lock entry section
    find_counter++; //report yourself as a reader 
    if (find_counter == 1) //checks if you're first reader
        pthread_mutex_lock(&resource_mutex); //if you're first, lock the database
    pthread_mutex_unlock(&find_mutex); //release entry section
    pthread_mutex_unlock(&block_mutex); // done trying access the database

    // database section reading
    Account to_find(acc_id); // demo account to be searched by id
    comare_for_find eq(&to_find); // building the compare object
    std::vector<Account*>::iterator it = (std::find_if(b_accs.begin(), b_accs.end(), eq));
    if (it == b_accs.end())
        ret = NULL;
    else
        ret = *(it);

    // exit section
    pthread_mutex_lock(&find_mutex); // Lock exit section
    find_counter--; // Indicate you're leaving
    if (find_counter == 0) // check if you're last
        pthread_mutex_unlock(&resource_mutex); // release database if you're last
    pthread_mutex_unlock(&find_mutex); // release exit section

    return ret;
}

/* Searches the accounts container for the related acount per id
 * Param 1 - account id to be searched
 * Returns - pointer to the relevant account
 *  if not found return b_accs.end() */
Account* find_acc_for_create(int acc_id)
{
    // this section is locked from atm.cpp in open account
    Account to_find(acc_id); // demo account to be searched by id
    comare_for_find eq(&to_find); // building the compare object
    std::vector<Account*>::iterator it = (std::find_if(b_accs.begin(), b_accs.end(), eq));
    if (it == b_accs.end())
        return NULL;
    else
        return *(it);
}
