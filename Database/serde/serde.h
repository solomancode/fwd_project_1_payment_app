#ifndef SERDE_H
#define SERDE_H

#include "../../Card/card.h"
#include "../../Server/server.h"
#include "../../Terminal/terminal.h"
#include "../../Utils/Common/common.h"

void parse_account_data(char *data, ST_accountsDB_t *out);
void parse_transaction_data(char *data, ST_transaction_t *out);

unsigned int parse_accounts_data(char *data, ST_accountsDB_t **accounts);
unsigned int parse_transactions_data(char *data, ST_transaction_t **transactions);

void serialize_card_data(ST_cardData_t *card, char_t out[]);
void serialize_terminal_data(ST_terminalData_t *term, char_t out[]);
void serialize_transaction(ST_transaction_t *tsx, char_t out[]);
void serialize_account(ST_accountsDB_t *account, char_t out[]);

#endif