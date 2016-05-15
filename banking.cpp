//TODO - add headers

int readcount, writecount; //(initial value = 0)
semaphore rmutex, wmutex, readTry, resource; //(initial value = 1)

typedef struct _account
{
    int acc_id;
    std::string pass;
    int balance;
    MyStruct(int k, const std::string& s) : key(k), stringValue(s) {}
} account;

/* Print balance information for account
 * Param 1 - pointer to account
 * Param 2 - password for that account */
void Read_acc( account *acc, string pass)
{

    readTry.P();//Indicate a reader is trying to enter

    rmutex.P();//lock entry section to avoid race condition with other readers
    readcount++;//report yourself as a reader
    if (readcount == 1)//checks if you are first reader
        resource.P();//if you are first reader, lock the resource
    rmutex.V();//release entry section for other readers

    readTry.V();//indicate you are done trying to access the resource

    // reading is performed

    rmutex.P();//reserve exit section - avoids race condition with readers
    readcount--;//indicate you're leaving
    if (readcount == 0)//checks if you are last reader leaving
        resource.V();//if last, you must release the locked resource
    rmutex.V();//release exit section for other readers
}

/* Doing transaction for account
 * Param 1 - pointer to account
 * Param 2 - password for that account
 * Param 3 - amount > 0 - depositing, amount < 0 - pulling */
void Write_acc(account *acc, string pass, int amount)
{

    wmutex.P();//reserve entry section for writers - avoids race conditions
    writecount++;//report yourself as a writer entering
    if (writecount == 1)//checks if you're first writer
        readTry.P();//if you're first, then you must lock the readers out. Prevent them from trying to enter CS
    wmutex.V();//release entry section


    resource.P();//reserve the resource for yourself - prevents other writers from simultaneously editing the shared resource
    // writing is performed
    resource.V();//release file


    wmutex.P();//reserve exit section
    writecount--;//indicate you're leaving
    if (writecount == 0)//checks if you're the last writer
    {
        if (readcount == 0)

        readTry.V();//if you're last writer, you must unlock the readers. Allows them to try enter CS for reading
    wmutex.V();//release exit section
}

/* Creates new account and updates the Container of accounts
 * Param 1 - account id to be created
 * Param 2 - password for that account
 * Param 3 - amount to initialize account with */
void Create_acc(int acc_id, string pass, int amount)
{
    cmutex.P(); // global mutex to lock all accounts
    createcount++;
    if (createcount == 1)
        block_all.P();
    cmutex.V();

    container.P();
    // insert to the vector and sort
    container.V();

    cmutex.P() 
    createcount--;
    if (createcount == 0)
        block_all.V();
    cmutex.V();
}

/* Searches the accounts container for the related acount per id
 * Param 1 - account id to be searched
 * Returns - pointer to the relevant account */
account* find_acc(int acc_id)
{
    block_all.P();

    find_mutex.P();
    findcount++;
    if (findcount == 1)
        container.P();
    find_mutex.V();
    
    block_all.V();
    // do search;

    find_mutex.P();
    findcount++;
    if (findcount == 0)
        container.V();
    find_mutex.V();

}

