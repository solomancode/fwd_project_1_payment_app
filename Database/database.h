#ifndef DATABASE_H
#define DATABASE_H

#include "../Card/card.h"
#include "../Utils/Filesystem/filesystem.h"
#include "serde/serde.h"

typedef enum
{
  FAILED,
  SUCCESS,
} Result;

typedef struct
{
  unsigned int accounts_count;
  unsigned int transactions_count;
  ST_accountsDB_t **accounts;
  ST_transaction_t **transactions;
} Database;

Database connect_database();

Result find_account(Database *db, char *PAN, ST_accountsDB_t *found);

Result find_transaction(Database *db, uint32_t sequenceNumber, ST_transaction_t *found);

Result insert_transaction(Database *db, ST_transaction_t *tsx);

void on_insert_transaction(Database *db, ST_transaction_t *tsx);

void read_accounts_table(ST_accountsDB_t **accounts, unsigned int *count);
#endif