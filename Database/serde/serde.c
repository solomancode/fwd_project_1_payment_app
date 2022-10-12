#include "serde.h"

void parse_transaction_data(char *data, ST_transaction_t *tsx)
{
    int col = 0;
    int offset = 0;
    char buffer[100] = {};
    int len = strlen(data);
    for (size_t i = 0; i < len; i++)
    {
        char current = data[i];
        if (current == ',')
        {
            switch (col)
            {
            case 0:
                tsx->transactionSequenceNumber = str_to_int(buffer);
                break;
            case 1:
                tsx->transState = str_to_int(buffer);
                break;
            case 2:
                string_copy(tsx->cardHolderData.primaryAccountNumber, buffer);
                break;
            case 3:
                string_copy(tsx->cardHolderData.cardHolderName, buffer);
                break;
            case 4:
                string_copy(tsx->cardHolderData.cardExpirationDate, buffer);
                break;
            case 5:
                string_copy(tsx->terminalData.transactionDate, buffer);
                break;
            case 6:
                tsx->terminalData.transAmount = atof(buffer);
                break;
            }
            col++;
            offset = 0;
            clear_array(buffer, 100);
        }
        else
        {
            buffer[offset] = current;
            offset++;
        };
    }
    tsx->terminalData.maxTransAmount = atof(buffer);
}

unsigned int parse_transactions_data(char *data, ST_transaction_t **transactions)
{
    char line[100] = {};
    int t_i = 0;
    int c_i = 0;
    for (size_t i = 0; i < strlen(data); i++)
    {
        char current = data[i];
        if (current == '\n')
        {
            ST_transaction_t *record = (ST_transaction_t *)malloc(sizeof(ST_transaction_t));
            parse_transaction_data(line, record);
            transactions[t_i] = record;
            t_i++;
            c_i = 0;
            clear_array(line, 99);
        }
        else
        {
            line[c_i] = current;
            c_i++;
        }
    }
    return t_i;
}

void insert_account(int i, char *pan, char *balance, ST_accountsDB_t **accounts)
{
    ST_accountsDB_t *record = malloc(sizeof(ST_accountsDB_t));
    record->balance = atoi(balance);
    strcpy(record->primaryAccountNumber, pan);
    accounts[i] = record;
}

unsigned int parse_accounts_data(char *data, ST_accountsDB_t **accounts)
{
    bool delim = false;
    int i_account = 0;
    int i_pan = 0;
    int i_balance = 0;
    char *pan = calloc(20, sizeof(char));
    char *balance = calloc(4, sizeof(char));
    for (size_t i = 0; i < strlen(data); i++)
    {
        char ch = data[i];
        if (ch == '\n' || ch == EOF)
        {
            insert_account(i_account, pan, balance, accounts);
            i_account++;
            pan = calloc(20, sizeof(char));
            balance = calloc(4, sizeof(char));
            i_pan = 0;
            i_balance = 0;
            delim = false;
            continue;
        };
        if (ch == ',')
        {
            delim = true;
            continue;
        }
        if (delim == false)
        {
            *(pan + i_pan) = ch;
            i_pan++;
        }
        else
        {
            *(balance + i_balance) = ch;
            i_balance++;
        }
    }
    insert_account(i_account, pan, balance, accounts);
    return i_account + 1;
}

void serialize_card_data(ST_cardData_t *card, char_t out[])
{
    sprintf(out, "%s,%s,%s", card->primaryAccountNumber, card->cardHolderName, card->cardExpirationDate);
}

void serialize_account(ST_accountsDB_t *account, char_t out[])
{
    char_t balance[5];
    float_to_string(account->balance, 4, balance);
    sprintf(out, "%s,%s\n", account->primaryAccountNumber, balance);
}

void serialize_terminal_data(ST_terminalData_t *term, char_t out[])
{
    size_t size = strlen(term->transactionDate) + 12;
    char_t amount[20];
    float_to_string(term->transAmount, DEC_FRAC, amount);
    char_t max_amount[20];
    float_to_string(term->maxTransAmount, DEC_FRAC, max_amount);
    snprintf(out, size, "%s,%s,%s", term->transactionDate, amount, max_amount);
}

void serialize_transaction(ST_transaction_t *tsx, char_t out[])
{
    char_t *ser_card = malloc(sizeof(ST_cardData_t));
    serialize_card_data(&tsx->cardHolderData, ser_card);
    char_t *ser_term = malloc(sizeof(ST_terminalData_t));
    serialize_terminal_data(&tsx->terminalData, ser_term);
    sprintf(out, "%u,%d,%s,%s\n", tsx->transactionSequenceNumber, tsx->transState, ser_card, ser_term);
    free(ser_card);
    free(ser_term);
}