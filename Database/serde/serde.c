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

void insert_account(int i, EN_accountState_t state, char *pan, char *balance, ST_accountsDB_t **accounts)
{
    ST_accountsDB_t *record = malloc(sizeof(ST_accountsDB_t));
    record->balance = atoi(balance);
    strcpy(record->primaryAccountNumber, pan);
    record->state = state;
    accounts[i] = record;
}

EN_accountState_t parse_account_state(char ch)
{
    if (ch == '1')
    {
        return BLOCKED;
    }
    else
    {
        return RUNNING;
    }
}

typedef enum
{
    ACCOUNT_STATE,
    ACCOUNT_PAN,
    ACCOUNT_BALANACE
} EnumParseAccountState;

unsigned int parse_accounts_data(char *data, ST_accountsDB_t **accounts)
{
    EnumParseAccountState parse_state = ACCOUNT_STATE;
    int account_index = 0;
    EN_accountState_t buffer_state;
    int buffer_pan_index = 0;
    char buffer_pan[50] = {0};
    int buffer_balance_index = 0;
    char buffer_balance[4] = {0};
    for (size_t i = 0; i < strlen(data); i++)
    {
        char ch = data[i];
        if (ch == '\n' || ch == EOF)
        {
            insert_account(account_index, buffer_state, buffer_pan, buffer_balance, accounts);

            for (size_t j = 0; j < buffer_pan_index; j++)
            {
                buffer_pan[j] = 0;
            }
            buffer_pan_index = 0;

            for (size_t j = 0; j < buffer_balance_index; j++)
            {
                buffer_balance[j] = 0;
            }
            buffer_balance_index = 0;

            parse_state = ACCOUNT_STATE;
            account_index++;

            continue;
        }

        if (ch == ',')
        {
            parse_state++;
            continue;
        }

        switch (parse_state)
        {
        case ACCOUNT_STATE:
            buffer_state = parse_account_state(ch);
            break;
        case ACCOUNT_PAN:
            buffer_pan[buffer_pan_index] = ch;
            buffer_pan_index++;
            break;
        case ACCOUNT_BALANACE:
            buffer_balance[buffer_balance_index] = ch;
            buffer_balance_index++;
            break;
        }
    }
    // insert_account(account_index, state, pan, balance, accounts);
    return account_index + 1;
}

void serialize_card_data(ST_cardData_t *card, char_t out[])
{
    sprintf(out, "%s,%s,%s", card->primaryAccountNumber, card->cardHolderName, card->cardExpirationDate);
}

void serialize_account(ST_accountsDB_t *account, char_t out[])
{
    char_t balance[5];
    float_to_string(account->balance, 4, balance);
    sprintf(out, "%d,%s,%s\n", account->state, account->primaryAccountNumber, balance);
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