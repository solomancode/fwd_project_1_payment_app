#include "terminal.h"
#include "terminal_input.h"

void simulate_setting_max_transaction_amount(ST_terminalData_t *termData)
{
    Select *select = select_create_empty(2, 50);
    select_add_item(select, green_text("use valid max transaction amount"));
    select_add_item(select, red_text("use invalid max transaction amount"));
    int selected = select_render(select);
    switch (selected)
    {
    case SIM_VALID_MAX_TSX_AMOUNT:
        termData->maxTransAmount = data_valid_max_tsx_amount;
        break;
    case SIM_INVALID_MAX_TSX_AMOUNT_VALUE:
        termData->maxTransAmount = data_invalid_max_tsx_amount;
        break;
    }
    select_cleanup(select);
}
void simulate_get_transaction_date_input(ST_terminalData_t *termData)
{
    Select *select = select_create_empty(2, 50);
    select_add_item(select, green_text("use valid transaction date ( system date )"));
    select_add_item(select, red_text("use invalid transaction date format"));
    int selected = select_render(select);
    switch (selected)
    {
    case SIM_VALID_TSX_DATE_FORMAT:
        Date today;
        date_get_today(&today);
        date_to_string_dd_mm_yyyy(&today, termData->transactionDate);
        break;
    case SIM_INVALID_TSX_DATE_FORMAT:
        string_copy(termData->transactionDate, data_invalid_date_format_expected_dd_mm_yy);
        break;
    }
    select_cleanup(select);
}

void simulate_reading_transaction_amount_input(ST_terminalData_t *termData)
{
    Select *select = select_create_empty(3, 50);
    select_add_item(select, green_text("use valid transaction amount input"));
    select_add_item(select, green_text("use valid transaction amount input ( exceeding max )"));
    select_add_item(select, red_text("use invalid transaction amount input"));
    int selected = select_render(select);
    switch (selected)
    {
    case SIM_VALID_TSX_AMOUNT_INPUT:
        termData->transAmount = data_valid_tsx_amount_input;
        break;
    case SIM_VALID_TSX_AMOUNT_INPUT_EXCEEDING_MAX:
        termData->transAmount = data_invalid_tsx_amount_exceeding;
        break;
    case SIM_INVALID_TSX_AMOUNT_INPUT:
        termData->transAmount = data_invalid_tsx_amount_input;
        break;
    }
    select_cleanup(select);
}