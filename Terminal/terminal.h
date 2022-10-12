#ifndef TERMINAL_H
#define TERMINAL_H

#include <stdint.h>
#include "../Utils/Select/select.h"
#include "../Utils/Date/date.h"
#include "../Utils/Common/common.h"
#include "../Card/card.h"
#include "../Utils/Console/console.h"

typedef struct ST_terminalData_t
{
    float transAmount;
    float maxTransAmount;
    uint8_t transactionDate[11];
} ST_terminalData_t;

typedef enum EN_terminalError_t
{
#ifndef OK_DUP
#define OK_DUP
    OK,
#endif
    WRONG_DATE = 1,
    EXPIRED_CARD,
    INVALID_CARD,
    INVALID_AMOUNT,
    EXCEED_MAX_AMOUNT,
    INVALID_MAX_AMOUNT
} EN_terminalError_t;

void simulate_processing_transaction_amount(ST_terminalData_t *termData);
void simulate_get_transaction_date_input(ST_terminalData_t *termData);
void simulate_setting_max_transaction_amount(ST_terminalData_t *termData);
void simulate_reading_transaction_amount_input(ST_terminalData_t *termData);

EN_terminalError_t getTransactionDate(ST_terminalData_t *termData);
EN_terminalError_t isCardExpired(ST_cardData_t cardData, ST_terminalData_t termData);
EN_terminalError_t isValidCardPAN(ST_cardData_t *cardData);
EN_terminalError_t getTransactionAmount(ST_terminalData_t *termData);
EN_terminalError_t isBelowMaxAmount(ST_terminalData_t *termData);
EN_terminalError_t setMaxAmount(ST_terminalData_t *termData);

#endif