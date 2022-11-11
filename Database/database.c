#include "database.h"

void read_accounts_table(ST_accountsDB_t **accounts, unsigned int *count)
{
    char *accounts_data = malloc(MAX_RW_FILE_BUFFER);
    EnumOpenFileResult open_or_err = read_file(db_accounts_path, accounts_data);
    if (open_or_err == OPEN_FILE_ERROR)
    {
        printf("%s\n", "Failed to read accounts data");
    }
    *count = parse_accounts_data((char *)accounts_data, accounts);
    free(accounts_data);
}

void read_transactions_table(ST_transaction_t **transactions, unsigned int *count)
{
    char *tsx_data = malloc(MAX_RW_FILE_BUFFER);
    bool read_or_false = read_file(db_transactions_path, tsx_data);
    if (read_or_false == OPEN_FILE_ERROR)
    {
        printf("%s\n", "Failed to read transactions data");
    }
    *count = parse_transactions_data((char *)tsx_data, transactions);
    free(tsx_data);
}

Database connect_database()
{
    unsigned int accounts_count = 0;
    ST_accountsDB_t **accounts = malloc(MAX_TABLE_RECORDS * sizeof(ST_accountsDB_t *));
    read_accounts_table(accounts, &accounts_count);
    unsigned int transactions_count = 0;
    ST_transaction_t **transactions = malloc(MAX_TABLE_RECORDS * sizeof(ST_transaction_t));
    read_transactions_table(transactions, &transactions_count);
    Database db = {
        accounts_count,
        transactions_count,
        accounts,
        transactions};
    return db;
}

Result find_account(Database *db, char *PAN, ST_accountsDB_t *found)
{
    Result result = FAILED;
    for (size_t i = 0; i < db->accounts_count; i++)
    {
        char *current = db->accounts[i]->primaryAccountNumber;
        // printf("find acc: %s == %s %s\n", PAN, current, strcmp(current, PAN) == 0 ? "M" : "!");
        if (strcmp(current, PAN) == 0)
        {
            string_copy(found->primaryAccountNumber, db->accounts[i]->primaryAccountNumber);
            found->balance = db->accounts[i]->balance;
            found->state = db->accounts[i]->state;
            return SUCCESS;
        }
    }
    return result;
}

Result insert_transaction(Database *db, ST_transaction_t *tsx)
{
    size_t size = sizeof(*tsx);
    db->transactions[db->transactions_count] = tsx;
    db->transactions_count++;
    char *table_data = calloc(db->transactions_count, size);
    for (size_t i = 0; i < db->transactions_count; i++)
    {
        tsx->transactionSequenceNumber = i;
        char data[500] = {};
        serialize_transaction(db->transactions[i], data);
        strcat(table_data, data);
    }
    write_file(db_transactions_path, table_data);
    free(table_data);
    return SUCCESS;
}

Result find_transaction(Database *db, uint32_t sequenceNumber, ST_transaction_t *found)
{
    for (size_t i = 0; i < db->transactions_count; i++)
    {
        ST_transaction_t *current = db->transactions[i];
        if (current->transactionSequenceNumber == sequenceNumber)
        {
            found->cardHolderData = current->cardHolderData;
            found->terminalData = current->terminalData;
            found->transactionSequenceNumber = current->transactionSequenceNumber;
            found->transState = current->transState;
            return SUCCESS;
        }
    }
    return FAILED;
}

Result update_account_balance(Database *db, char_t pan[], float amount)
{
    for (size_t i = 0; i < db->accounts_count; i++)
    {
        if (strcmp(pan, db->accounts[i]->primaryAccountNumber) == 0)
        {
            db->accounts[i]->balance -= amount;
        }
    }
    size_t size = sizeof(*db->accounts[0]);
    char_t *table_data = (char_t *)calloc(db->accounts_count, size);
    for (size_t i = 0; i < db->accounts_count - 1; i++)
    {
        char_t data[500];
        serialize_account(db->accounts[i], data);
        strcat(table_data, data);
    }
    write_file(db_accounts_path, table_data);
}

void on_insert_transaction(Database *db, ST_transaction_t *tsx)
{
    if (tsx->transState == APPROVED)
    {
        update_account_balance(db, tsx->cardHolderData.primaryAccountNumber, tsx->terminalData.transAmount);
    }
}
