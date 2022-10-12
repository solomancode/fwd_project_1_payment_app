#include "terminal.h"

EN_terminalError_t getTransactionDate(ST_terminalData_t *termData)
{
    simulate_get_transaction_date_input(termData);
    print_key_value("transactionDate", termData->transactionDate);
    Date date;
    EnumDateParseResult result = date_parse_date_string_dd_mm_yyyy(termData->transactionDate, &date);
    return result == DATE_PARSE_OK ? OK : WRONG_DATE;
}

EN_terminalError_t isCardExpired(ST_cardData_t cardData, ST_terminalData_t termData)
{
    Date card_date;
    date_parse_date_string_mm_yy((char_t *)&cardData.cardExpirationDate, &card_date);
    Date tsx_date;
    date_parse_date_string_dd_mm_yyyy((char_t *)&termData.transactionDate, &tsx_date);
    return date_compare_dates(&card_date, &tsx_date) == DATE_1_GT_DATE_2 ? OK : EXPIRED_CARD;
}

EN_terminalError_t isValidCardPAN(ST_cardData_t *cardData)
{
    // optional
}

EN_terminalError_t getTransactionAmount(ST_terminalData_t *termData)
{
    simulate_reading_transaction_amount_input(termData);
    char amount[9] = {};
    float_to_string(termData->transAmount, 2, amount);
    print_key_value("transAmount", amount);
    return termData->transAmount > 0 ? OK : INVALID_AMOUNT;
}

EN_terminalError_t isBelowMaxAmount(ST_terminalData_t *termData)
{
    return termData->transAmount <= termData->maxTransAmount ? OK : EXCEED_MAX_AMOUNT;
}

EN_terminalError_t setMaxAmount(ST_terminalData_t *termData)
{
    simulate_setting_max_transaction_amount(termData);
    char max[9] = {};
    float_to_string(termData->maxTransAmount, 2, max);
    print_key_value("maxTransAmount", max);
    return termData->maxTransAmount > 0 ? OK : INVALID_MAX_AMOUNT;
}