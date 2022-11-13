#ifndef CONSTANTS_H
#define CONSTANTS_H

#define MAX_RW_FILE_BUFFER 1000000
#define MAX_TABLE_RECORDS 255
#define DEC_FRAC 4

#define data_valid_card_holder_name "Mohamed Adel Mohamed"
#define data_invalid_card_holder_name_lt_20 ""
#define data_invalid_card_holder_name_gt_24 "soooooo long invalid card holder name"

#define data_valid_expiry_date "06/27"
#define data_invalid_date_format_expected_mm_yy "06/2027"
#define data_valid_expired_date "06/10"

#define data_valid_pan "5437525041975812"
#define data_invalid_pan_lt_16 ""
#define data_invalid_pan_gt_19 "55425233430109903425233430109903"

#define data_stolen_card_pan "9437525041975812"
#define data_fraud_card_pan "0000000000000000"

#define data_invalid_date_format_expected_dd_mm_yy "06-2022"

#define data_valid_tsx_amount_input 1
#define data_invalid_tsx_amount_input 0

#define data_valid_tsx_amount 1
#define data_invalid_tsx_amount_value 0
#define data_invalid_tsx_amount_exceeding 10000000

#define data_valid_max_tsx_amount 99
#define data_invalid_max_tsx_amount 0

#define db_accounts_path "Database/data/accounts.csv"
#define db_transactions_path "Database/data/transactions.csv"

#endif