#include "card.h"

EN_cardError_t getCardHolderName(ST_cardData_t *cardData)
{
    simulate_reading_card_name(cardData);
    print_key_value("cardHolderName", cardData->cardHolderName);
    return validate_input_min_max(cardData->cardHolderName, 20, 24) ? OK : WRONG_NAME;
}

EN_cardError_t getCardPAN(ST_cardData_t *cardData)
{
    simulate_reading_pan(cardData);
    print_key_value("primaryAccountNumber", cardData->primaryAccountNumber);
    return validate_input_min_max(cardData->primaryAccountNumber, 16, 18) ? OK : WRONG_PAN;
}

EN_cardError_t getCardExpiryDate(ST_cardData_t *cardData)
{
    simulate_reading_card_expiry_date_format(cardData);
    print_key_value("cardExpirationDate", cardData->cardExpirationDate);
    Date date;
    EnumDateParseResult result = date_parse_date_string_mm_yy(cardData->cardExpirationDate, &date);
    return result == DATE_PARSE_OK ? OK : WRONG_EXP_DATE;
}