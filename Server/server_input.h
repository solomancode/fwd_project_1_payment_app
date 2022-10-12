#ifndef SERVER_INPUT_H
#define SERVER_INPUT_H

#include "server.h"

typedef enum
{
    SIM_VALID_AVAILABLE_TSX_AMOUNT = 1,
    SIM_INVALID_UNAVAILABLE_TSX_AMOUNT,
} CardAmountAvailableOptions;

typedef enum
{
    SIM_VALID_TSX_AMOUNT = 1,
    SIM_INVALID_TSX_AMOUNT_EXCEEDING
} TsxAmountOptions;

#endif