#ifndef TERMINAL_INPUT_H
#define TERMINAL_INPUT_H

typedef enum
{
    SIM_VALID_MAX_TSX_AMOUNT = 1,
    SIM_INVALID_MAX_TSX_AMOUNT_VALUE,
} ValidTsxAmountOptions;

typedef enum
{
    SIM_VALID_TSX_DATE_FORMAT = 1,
    SIM_INVALID_TSX_DATE_FORMAT,
} TsxDateOptions;

typedef enum
{
    SIM_VALID_CARD_DATE = 1,
    SIM_INVALID_EXPIRED_CARD_DATE,
} CardExpiryOptions;

typedef enum
{
    SIM_VALID_TSX_AMOUNT_INPUT = 1,
    SIM_VALID_TSX_AMOUNT_INPUT_EXCEEDING_MAX,
    SIM_INVALID_TSX_AMOUNT_INPUT,
} TsxAmountInputOptions;

#endif