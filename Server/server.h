#ifndef SERVER_H
#define SERVER_H

#include "../Card/card.h"
#include "../Terminal/terminal.h"

typedef enum EN_accountState_t
{
    RUNNING,
    BLOCKED
} EN_accountState_t;

typedef enum EN_transState_t
{
    APPROVED,
    DECLINED_INSUFFICIENT_FUND,
    DECLINED_STOLEN_CARD,
    INTERNAL_SERVER_ERROR,
    FRAUD_CARD
} EN_transState_t;

typedef struct ST_transaction_t
{
    ST_cardData_t cardHolderData;
    ST_terminalData_t terminalData;
    EN_transState_t transState;
    uint32_t transactionSequenceNumber;
} ST_transaction_t;

typedef enum EN_serverError_t
{
#ifndef OK_DUP
#define OK_DUP
    OK,
#endif
    SAVING_FAILED = 1,
    TRANSACTION_NOT_FOUND,
    ACCOUNT_NOT_FOUND,
    LOW_BALANCE,

} EN_serverError_t;

typedef struct ST_accountsDB_t
{
    float balance;
    EN_accountState_t state;
    uint8_t primaryAccountNumber[20];
} ST_accountsDB_t;

void simulate_validate_available_amount(ST_terminalData_t *termData);
void simulate_validate_card_account(ST_cardData_t *cardData);

EN_transState_t receiveTransactionData(ST_transaction_t *transData);
EN_serverError_t isValidAccount(ST_cardData_t *cardData);
EN_serverError_t isAmountAvailable(ST_terminalData_t *termData);
EN_serverError_t saveTransaction(ST_transaction_t *transData);
EN_serverError_t getTransaction(uint32_t transactionSequenceNumber, ST_transaction_t *transData);
EN_serverError_t isBlockedAccount(ST_accountsDB_t *accountRefrence);

#endif