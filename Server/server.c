#include "server.h"
#include "../Database/database.h"

EN_serverError_t isBlockedAccount(ST_accountsDB_t *accountRefrence)
{
    return accountRefrence->state == BLOCKED ? ACCOUNT_NOT_FOUND : OK;
}

EN_transState_t receiveTransactionData(ST_transaction_t *transData)
{
    Database db = connect_database();
    ST_accountsDB_t found;
    Result is_found = find_account(&db, transData->cardHolderData.primaryAccountNumber, &found);
    if (is_found == FAILED)
    {
        transData->transState = FRAUD_CARD;
        EN_serverError_t saved_transaction_or_err = saveTransaction(transData);
        return (saved_transaction_or_err == INTERNAL_SERVER_ERROR) ? INTERNAL_SERVER_ERROR : FRAUD_CARD;
    }
    else
    {
        EN_serverError_t valid_account_or_err = isBlockedAccount(&found);
        if (valid_account_or_err == ACCOUNT_NOT_FOUND)
        {
            transData->transState = DECLINED_STOLEN_CARD;
            EN_serverError_t saved_transaction_or_err = saveTransaction(transData);
            return (saved_transaction_or_err == INTERNAL_SERVER_ERROR) ? INTERNAL_SERVER_ERROR : DECLINED_STOLEN_CARD;
        }

        EN_serverError_t valid_amount_or_err = isAmountAvailable(&transData->terminalData);
        if (valid_amount_or_err == LOW_BALANCE)
        {
            transData->transState = DECLINED_INSUFFICIENT_FUND;
            EN_serverError_t saved_transaction_or_err = saveTransaction(transData);
            return (saved_transaction_or_err == INTERNAL_SERVER_ERROR) ? INTERNAL_SERVER_ERROR : DECLINED_INSUFFICIENT_FUND;
        }

        transData->transState = APPROVED;
        EN_serverError_t saved_transaction_or_err = saveTransaction(transData);
        return (saved_transaction_or_err == INTERNAL_SERVER_ERROR) ? INTERNAL_SERVER_ERROR : APPROVED;
    }
}

EN_serverError_t isAmountAvailable(ST_terminalData_t *termData)
{
    simulate_processing_transaction_amount(termData);
    Database db = connect_database();
    ST_accountsDB_t found;
    find_account(&db, data_valid_pan, &found);
    print_key_value("primaryAccountNumber", found.primaryAccountNumber);
    char balance[20];
    float_to_string(found.balance, 2, balance);
    char tsx_amount[20];
    float_to_string(termData->transAmount, 9, tsx_amount);
    print_key_value("transAmount", tsx_amount);
    print_key_value("balance", balance);
    return found.balance - termData->transAmount >= 0 ? OK : LOW_BALANCE;
}

EN_serverError_t saveTransaction(ST_transaction_t *transData)
{
    Database db = connect_database();
    print_title("💾 Saving Transaction");
    Result result_tsx = insert_transaction(&db, transData);
    on_insert_transaction(&db, transData);
    return result_tsx == SUCCESS ? OK : INTERNAL_SERVER_ERROR;
}

EN_serverError_t getTransaction(uint32_t transactionSequenceNumber, ST_transaction_t *transData)
{
    Database db = connect_database();
    Result result = find_transaction(&db, transactionSequenceNumber, transData);
    return result == SUCCESS ? OK : TRANSACTION_NOT_FOUND;
}