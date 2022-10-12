#include "server_input.h"

void simulate_processing_transaction_amount(ST_terminalData_t *termData)
{
    Select *select = select_create_empty(3, sizeof(char) * 50);
    select_add_item(select, green_text("use valid transaction amount"));
    select_add_item(select, green_text("use valid transaction amount ( exceeding balance )"));
    int selected = select_render(select);
    switch (selected)
    {
    case SIM_VALID_TSX_AMOUNT:
        termData->transAmount = data_valid_tsx_amount;
        break;
    case SIM_INVALID_TSX_AMOUNT_EXCEEDING:
        termData->transAmount = data_invalid_tsx_amount_exceeding;
        break;
    }

    select_cleanup(select);
}

void simulate_validate_available_amount(ST_terminalData_t *termData)
{
    Select *select = select_create_empty(2, 50);
    select_add_item(select, green_text("use valid available transaction amount"));
    select_add_item(select, red_text("use invalid unavailable transaction amount"));
    int selected = select_render(select);
    switch (selected)
    {
    case SIM_VALID_AVAILABLE_TSX_AMOUNT:
        termData->transAmount = data_valid_tsx_amount;
        break;
    case SIM_INVALID_UNAVAILABLE_TSX_AMOUNT:
        termData->transAmount = data_invalid_tsx_amount_exceeding;
        break;
    }
    select_cleanup(select);
}