#ifndef CARD_H
#define CARD_H

#include "../constants.h"
#include "../Utils/Validation/validation.h"
#include "../Utils/Select/select.h"
#include "../Utils/Date/date.h"
#include "../Utils/Console/console.h"

typedef struct ST_cardData_t
{
    uint8_t cardHolderName[25];
    uint8_t primaryAccountNumber[20];
    uint8_t cardExpirationDate[6];
} ST_cardData_t;

typedef enum EN_cardError_t
{
#ifndef OK_DUP
#define OK_DUP
    OK,
#endif
    WRONG_NAME = 1,
    WRONG_EXP_DATE,
    WRONG_PAN
} EN_cardError_t;

void simulate_reading_card_name(ST_cardData_t *card_data);
int simulate_reading_card_expiry_date_format(ST_cardData_t *cardData);
int simulate_reading_pan(ST_cardData_t *cardData);

EN_cardError_t getCardHolderName(ST_cardData_t *cardData);
EN_cardError_t getCardExpiryDate(ST_cardData_t *cardData);
EN_cardError_t getCardPAN(ST_cardData_t *cardData);

#endif