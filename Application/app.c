#include "../Card/card.h"
#include "../Server/server.h"
#include "../Terminal/terminal.h"
#include "../Utils/Console/console.h"
#include "../Utils/Common/common.h"
#include "../Database/database.h"

void exit_on_error(bool is_error, char_t *error)
{
    if (is_error)
    {
        print_error(error);
        exit(EXIT_FAILURE);
    }
}

ST_transaction_t *test_setup()
{
    ST_cardData_t cardData = {
        data_valid_card_holder_name,
        data_valid_pan,
        data_valid_expired_date,
    };

    Date today;
    date_get_today(&today);

    ST_terminalData_t termData = {
        data_valid_tsx_amount,
        data_valid_max_tsx_amount,
        data_valid_expiry_date,
    };

    date_to_string_dd_mm_yyyy(&today, termData.transactionDate);

    ST_transaction_t *tsx = calloc(1, sizeof(ST_transaction_t));
    tsx->cardHolderData = cardData;
    tsx->terminalData = termData;
    tsx->transState = APPROVED;
    tsx->transactionSequenceNumber = 0;

    return tsx;
}

void test_isBlockedAccount()
{
    ST_transaction_t *tsx = test_setup();
    EN_cardError_t card_pan_or_err = getCardPAN(&tsx->cardHolderData);
    exit_on_error(card_pan_or_err == WRONG_PAN, "Invalid PAN");

    Database db = connect_database();
    ST_accountsDB_t found;
    Result is_found = find_account(&db, tsx->cardHolderData.primaryAccountNumber, &found);
    if (is_found == FAILED)
    {
        print_error("Account is Blocked ( BLOCKED )");
        return;
    }
    EN_serverError_t result = isBlockedAccount(&found);
    if (result == RUNNING)
    {
        print_ok("Account is Valid ( RUNNING )");
    }
    else
    {
        print_error("Account is Blocked ( BLOCKED )");
    };

    free(tsx);
}

void test_receiveTransactionData()
{
    ST_transaction_t *tsx = test_setup();
    EN_cardError_t card_holder_name_or_err = getCardHolderName(&tsx->cardHolderData);
    exit_on_error(card_holder_name_or_err == WRONG_NAME, "Invalid Card Name");

    print_ok("Valid Card Name");

    EN_cardError_t card_expiry_or_err = getCardExpiryDate(&tsx->cardHolderData);
    exit_on_error(card_expiry_or_err == WRONG_EXP_DATE, "Invalid Expiry Date");

    print_ok("Valid Expiry Name");

    EN_cardError_t card_pan_or_err = getCardPAN(&tsx->cardHolderData);
    exit_on_error(card_pan_or_err == WRONG_PAN, "Invalid PAN");

    print_ok("Valid PAN");

    EN_terminalError_t tsx_date_or_err = getTransactionDate(&tsx->terminalData);
    exit_on_error(tsx_date_or_err == WRONG_DATE, "Invalid Transaction Date");

    print_ok("Valid Transaction Date");

    EN_terminalError_t not_expired_or_err = isCardExpired(tsx->cardHolderData, tsx->terminalData);
    exit_on_error(not_expired_or_err == EXPIRED_CARD, "Declined Expired Card");

    print_ok("Card Not Expired");

    EN_terminalError_t valid_max_input_or_err = setMaxAmount(&tsx->terminalData);
    exit_on_error(valid_max_input_or_err == INVALID_MAX_AMOUNT, "Invalid Maximum Amount Input");

    print_ok("Valid Maximum Amount");

    EN_terminalError_t valid_tsx_input_or_err = getTransactionAmount(&tsx->terminalData);
    exit_on_error(valid_tsx_input_or_err == INVALID_AMOUNT, "Invalid Transaction Amount Input");

    print_ok("Valid Transaction Amount");

    EN_terminalError_t valid_max_amount_or_err = isBelowMaxAmount(&tsx->terminalData);
    exit_on_error(valid_max_amount_or_err == EXCEED_MAX_AMOUNT, "Declined Amount Exceeded Limit");

    print_ok("Transaction Amount Below Max");

    EN_serverError_t valid_tsx_or_err = receiveTransactionData(tsx);

    switch (valid_tsx_or_err)
    {

    case FRAUD_CARD:
        print_error("Fraud Card");
        break;

    case DECLINED_STOLEN_CARD:
        print_error("Declined Invalid Account");
        break;

    case DECLINED_INSUFFICIENT_FUND:
        print_error("Declined Insufficient Funds");
        break;

    case INTERNAL_SERVER_ERROR:
        print_error("Internal Server Error");
        break;
    }

    free(tsx);
}

EN_serverError_t test_getTransaction(uint32_t sn)
{
    ST_transaction_t *tsx = test_setup();
    EN_serverError_t err_or_tsx = getTransaction(sn, tsx);
    if (err_or_tsx == TRANSACTION_NOT_FOUND)
    {
        print_error("Transaction Not Found");
        return TRANSACTION_NOT_FOUND;
    }
    print_ok("Found Transaction");
    printf("%u\n", tsx->transactionSequenceNumber);
    free(tsx);
}

void appStart(void)
{
    test_getTransaction(10);
}