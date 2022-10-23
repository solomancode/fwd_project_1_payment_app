#include "../Card/card.h"
#include "../Server/server.h"
#include "../Terminal/terminal.h"
#include "../Utils/Console/console.h"
#include "../Utils/Common/common.h"

void exit_on_error(bool is_error, char_t *error)
{
    if (is_error)
    {
        print_error(error);
        exit(EXIT_FAILURE);
    }
}

void report_stolen_card(ST_transaction_t *tsx)
{
    tsx->transState = DECLINED_STOLEN_CARD;
    saveTransaction(tsx);
    exit_on_error(true, "Declined Invalid Account");
}

void report_low_balance(ST_transaction_t *tsx)
{
    tsx->transState = DECLINED_INSUFFICIENT_FUND;
    saveTransaction(tsx);
    exit_on_error(true, "Declined Insufficient Funds");
}

void appStart(void)
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

    ST_transaction_t tsx = {
        cardData,
        termData,
        APPROVED,
        0,
    };

    EN_cardError_t card_holder_name_or_err = getCardHolderName(&tsx.cardHolderData);
    exit_on_error(card_holder_name_or_err == WRONG_NAME, "Invalid Card Name");

    print_ok("Valid Card Name");

    EN_cardError_t card_expiry_or_err = getCardExpiryDate(&tsx.cardHolderData);
    exit_on_error(card_expiry_or_err == WRONG_EXP_DATE, "Invalid Expiry Date");

    print_ok("Valid Expiry Name");

    EN_cardError_t card_pan_or_err = getCardPAN(&tsx.cardHolderData);
    exit_on_error(card_pan_or_err == WRONG_PAN, "Invalid PAN");

    print_ok("Valid PAN");

    EN_terminalError_t tsx_date_or_err = getTransactionDate(&tsx.terminalData);
    exit_on_error(tsx_date_or_err == WRONG_DATE, "Invalid Transaction Date");

    print_ok("Valid Transaction Date");

    EN_terminalError_t not_expired_or_err = isCardExpired(tsx.cardHolderData, tsx.terminalData);
    exit_on_error(not_expired_or_err == EXPIRED_CARD, "Declined Expired Card");

    print_ok("Card Not Expired");

    EN_terminalError_t valid_max_input_or_err = setMaxAmount(&tsx.terminalData);
    exit_on_error(valid_max_input_or_err == INVALID_MAX_AMOUNT, "Invalid Maximum Amount Input");

    print_ok("Valid Maximum Amount");

    EN_terminalError_t valid_tsx_input_or_err = getTransactionAmount(&tsx.terminalData);
    exit_on_error(valid_tsx_input_or_err == INVALID_AMOUNT, "Invalid Transaction Amount Input");

    print_ok("Valid Transaction Amount");

    EN_terminalError_t valid_max_amount_or_err = isBelowMaxAmount(&tsx.terminalData);
    exit_on_error(valid_max_amount_or_err == EXCEED_MAX_AMOUNT, "Declined Amount Exceeded Limit");

    print_ok("Transaction Amount Below Max");

    EN_serverError_t valid_pan_or_err = isValidAccount(&tsx.cardHolderData);
    if (valid_pan_or_err == ACCOUNT_NOT_FOUND)
        report_stolen_card(&tsx);

    print_ok("Account is Valid");

    EN_serverError_t valid_tsx_amount_or_err = isAmountAvailable(&tsx.terminalData);
    if (valid_tsx_amount_or_err == LOW_BALANCE)
        report_low_balance(&tsx);

    EN_serverError_t valid_tsx_or_err = receiveTransactionData(&tsx);

    switch (valid_tsx_or_err)
    {
    case DECLINED_STOLEN_CARD:
        report_stolen_card(&tsx);
        break;

    case DECLINED_INSUFFICIENT_FUND:
        report_low_balance(&tsx);
        break;

    case INTERNAL_SERVER_ERROR:
        exit_on_error(true, "Internal Server Error");
        break;

    default:
        print_ok("Valid Transaction");
    }

    EN_serverError_t save_or_err = saveTransaction(&tsx);
    exit_on_error(save_or_err == INTERNAL_SERVER_ERROR, "Internal Server Error");
}