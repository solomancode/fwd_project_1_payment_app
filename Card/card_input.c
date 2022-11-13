#include "card.h"
#include "card_input.h"

void simulate_reading_card_name(ST_cardData_t *card_data)
{
    Select *select = select_create_empty(3, 50);
    select_add_item(select, green_text("use valid card name"));
    select_add_item(select, red_text("use invalid card name < 20"));
    select_add_item(select, red_text("use invalid card name > 24"));
    int selected = select_render(select);
    switch (selected)
    {
    case SIM_VALID_CARD_HOLDER_NAME:
        string_copy(card_data->cardHolderName, data_valid_card_holder_name);
        break;
    case SIM_INVALID_CARD_HOLDER_NAME_LT_20:
        string_copy(card_data->cardHolderName, data_invalid_card_holder_name_lt_20);
        break;
    case SIM_INVALID_CARD_HOLDER_NAME_GT_24:
        string_copy(card_data->cardHolderName, data_invalid_card_holder_name_gt_24);
        break;
    }
    select_cleanup(select);
}

int simulate_reading_pan(ST_cardData_t *cardData)
{
    Select *select = select_create_empty(5, 20);
    select_add_item(select, green_text("use valid PAN"));
    select_add_item(select, green_text("use valid PAN ( fraud card )"));
    select_add_item(select, green_text("use valid PAN ( stolen card )"));
    select_add_item(select, red_text("use invalid PAN < 16"));
    select_add_item(select, red_text("use invalid PAN > 19"));
    int selected = select_render(select);
    switch (selected)
    {
    case SIM_VALID_PAN:
        string_copy(cardData->primaryAccountNumber, data_valid_pan);
        break;
    case SIM_VALID_PAN_FRAUD_CARD:
        string_copy(cardData->primaryAccountNumber, data_fraud_card_pan);
        break;
    case SIM_VALID_PAN_STOLEN_CARD:
        string_copy(cardData->primaryAccountNumber, data_stolen_card_pan);
        break;
    case SIM_INVALID_PAN_LT_16:
        string_copy(cardData->primaryAccountNumber, data_invalid_pan_lt_16);
        break;
    case SIM_INVALID_PAN_GT_19:
        string_copy(cardData->primaryAccountNumber, data_invalid_pan_gt_19);
        break;
    }
    select_cleanup(select);
}

int simulate_reading_card_expiry_date_format(ST_cardData_t *cardData)
{
    Select *select = select_create_empty(3, 50);
    select_add_item(select, green_text("use valid expiry date format"));
    select_add_item(select, green_text("use valid date format ( expired )"));
    select_add_item(select, red_text("use invalid expiry date format"));
    int selected = select_render(select);
    switch (selected)
    {
    case SIM_VALID_EXPIRY_DATE_FORMAT:
        string_copy(cardData->cardExpirationDate, data_valid_expiry_date);
        break;
    case SIM_VALID_EXPIRY_DATE_FORMAT_EXPIRED:
        string_copy(cardData->cardExpirationDate, data_valid_expired_date);
        break;
    case SIM_INVALID_EXPIRY_DATE_FORMAT:
        string_copy(cardData->cardExpirationDate, data_invalid_date_format_expected_mm_yy);
        break;
    }
    select_cleanup(select);
}