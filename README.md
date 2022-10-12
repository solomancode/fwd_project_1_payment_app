**Input data can be found in constants.h**

### getCardHolderName

[getCardHolderName.webm](https://user-images.githubusercontent.com/7840989/195370253-64f37fca-a9d5-48fc-96b3-be3bf6890f73.webm)

The following function displays a list of input options

1. use valid card name
2. use invalid card name < 20
3. use invalid card name > 24

it prints the selected input value, then validates the input value

- validate_input_min_max is a helper function that checks if a string
matches a specific range, i.e. 20 : 24 inclusive.

```c
EN_cardError_t getCardHolderName(ST_cardData_t *cardData)
{
    simulate_reading_card_name(cardData);
    print_key_value("cardHolderName", cardData->cardHolderName);
    return validate_input_min_max(cardData->cardHolderName, 20, 24) ? OK : WRONG_NAME;
}
```


### getCardExpiryDate

[getCardExpiryDate.webm](https://user-images.githubusercontent.com/7840989/195373151-9bf17cdd-5e31-4ed7-8432-830f3daabbad.webm)

The following function displays a list of input options

1. use valid expiry date format
2. use valid date format ( expired )
3. use invalid expiry date format

After receiving user input a call to parse the input is made,
the parser validates the input value, parses it's value and returns
ok if successful

```c
EN_cardError_t getCardExpiryDate(ST_cardData_t *cardData)
{
    simulate_reading_card_expiry_date_format(cardData);
    print_key_value("cardExpirationDate", cardData->cardExpirationDate);
    Date date;
    EnumDateParseResult result = date_parse_date_string_mm_yy(cardData->cardExpirationDate, &date);
    return result == DATE_PARSE_OK ? OK : WRONG_EXP_DATE;
}
```
### getCardPAN

[getCardPAN.webm](https://user-images.githubusercontent.com/7840989/195374685-2ba1bc86-fadc-45e4-9d70-4afc95c9071c.webm)

The following function displays a list of input options

1. use valid PAN
2. use valid PAN ( stolen card )
3. use invalid PAN < 16
4. use invalid PAN > 19

- validate_input_min_max is a helper function that checks if a string
matches a specific range, i.e. 20 : 24 inclusive.

```c
EN_cardError_t getCardPAN(ST_cardData_t *cardData)
{
    simulate_reading_pan(cardData);
    print_key_value("primaryAccountNumber", cardData->primaryAccountNumber);
    return validate_input_min_max(cardData->primaryAccountNumber, 16, 18) ? OK : WRONG_PAN;
}
```

### getTransactionDate

[getTransactionDate.webm](https://user-images.githubusercontent.com/7840989/195375773-da8a0fcb-e70c-4eaa-9bdb-089dd30529e9.webm)

The following function displays a list of input options

1. use valid transaction date ( system date )
2. use invalid transaction date format

After receiving user input a call to parse the input is made,
the parser validates the input value, parses it's value and returns
ok if successful

```c
EN_terminalError_t getTransactionDate(ST_terminalData_t *termData)
{
    simulate_get_transaction_date_input(termData);
    print_key_value("transactionDate", termData->transactionDate);
    Date date;
    EnumDateParseResult result = date_parse_date_string_dd_mm_yyyy(termData->transactionDate, &date);
    return result == DATE_PARSE_OK ? OK : WRONG_DATE;
}
```

### isCardExpired 

[isCardExpired.webm](https://user-images.githubusercontent.com/7840989/195377730-9d55c0f1-4fd9-428b-8b7d-7d0b7074ce1c.webm)

The following function receives card expiry date and transaction date

- date_compare_dates is a helper function that compares two dates, card is not
expired if the card date is greater than transaction date.


```c
EN_terminalError_t isCardExpired(ST_cardData_t cardData, ST_terminalData_t termData)
{
    Date card_date;
    date_parse_date_string_mm_yy((char_t *)&cardData.cardExpirationDate, &card_date);
    Date tsx_date;
    date_parse_date_string_dd_mm_yyyy((char_t *)&termData.transactionDate, &tsx_date);
    return date_compare_dates(&card_date, &tsx_date) == DATE_1_GT_DATE_2 ? OK : EXPIRED_CARD;
}
```

### setMaxAmount

The following function displays a list of input options

[setMaxAmount.webm](https://user-images.githubusercontent.com/7840989/195378781-bea8b003-5259-43ee-8e6a-cd335f6446bc.webm)

1. use valid max transaction amount
2. use invalid max transaction amount

Max amount is valid if is greater than 0

```c
EN_terminalError_t setMaxAmount(ST_terminalData_t *termData)
{
    simulate_setting_max_transaction_amount(termData);
    char max[9] = {};
    float_to_string(termData->maxTransAmount, 2, max);
    print_key_value("maxTransAmount", max);
    return termData->maxTransAmount > 0 ? OK : INVALID_MAX_AMOUNT;
}
```

### getTransactionAmount

[getTransactionAmount.webm](https://user-images.githubusercontent.com/7840989/195379708-daa5c101-483a-4a87-b02b-d51e16e594d4.webm)

The following function displays a list of input options

1. use valid transaction amount input
2. use valid transaction amount input ( exceeding max )
3. use invalid transaction amount input

Transaction amount is valid if is greater than 0

```c
EN_terminalError_t getTransactionAmount(ST_terminalData_t *termData)
{
    simulate_reading_transaction_amount_input(termData);
    char amount[9] = {};
    float_to_string(termData->transAmount, 2, amount);
    print_key_value("transAmount", amount);
    return termData->transAmount > 0 ? OK : INVALID_AMOUNT;
}
```

### isBelowMaxAmount 

[isBelowMaxAmount.webm](https://user-images.githubusercontent.com/7840989/195380825-670fdf37-89de-492a-9b04-53fdd5d3bd83.webm)

The following function receives max amount limit and desired transaction amount and
checks if the desired transaction is below the max limit

```c
EN_terminalError_t isBelowMaxAmount(ST_terminalData_t *termData)
{
    return termData->transAmount <= termData->maxTransAmount ? OK : EXCEED_MAX_AMOUNT;
}
```

### isValidAccount

[isValidAccount.webm](https://user-images.githubusercontent.com/7840989/195385393-2e67d4c6-4ce1-47c0-84ac-eef69364839a.webm)

The following function receives card data, establishes a connection
to the database to find the corresponding account pan, If found the 
function will return success, OK. If not found the incident will be
reported and a new record will be created at the transaction table.

```c
EN_serverError_t isValidAccount(ST_cardData_t *cardData)
{
    print_title("🔎 Validating Card PAN...");
    print_key_value("primaryAccountNumber", cardData->primaryAccountNumber);
    return validate_card_pan(cardData->primaryAccountNumber) ? OK : ACCOUNT_NOT_FOUND;
}

bool validate_card_pan(char *pan)
{
    Database db = connect_database();
    ST_accountsDB_t found;
    Result is_found = find_account(&db, pan, &found);
    return is_found == SUCCESS;
}
```

### isAmountAvailable

[isAmountAvailable.webm](https://user-images.githubusercontent.com/7840989/195391911-fc2add05-e1a8-4169-9888-26ecc081404b.webm)

The following function receives terminal data, establishes a connection
to the database to find the current balance and compares it with the required
transaction amount, if found balance will not suffice the transaction, the 
transaction will be reported with status: Insufficient Funds.

```c
EN_serverError_t isAmountAvailable(ST_terminalData_t *termData)
{
    simulate_processing_transaction_amount(termData);
    Database db = connect_database();
    ST_accountsDB_t found;
    find_account(&db, data_valid_pan, &found);
    print_key_value("primaryAccountNumber", found.primaryAccountNumber);
    char balance[20];
    float_to_string(found.balance, 2, balance);
    char tsx_amount[20];
    float_to_string(termData->transAmount, 9, tsx_amount);
    print_key_value("transAmount", tsx_amount);
    print_key_value("balance", balance);
    return found.balance - termData->transAmount >= 0 ? OK : LOW_BALANCE;
}
```

### receiveTransactionData

[receiveTransactionData.webm](https://user-images.githubusercontent.com/7840989/195397494-f46b19de-c86a-42d6-83c0-053ab628d5cc.webm)

The following function receives transaction data and performs pre-order
checks to validate transaction data

```c
EN_transState_t receiveTransactionData(ST_transaction_t *transData)
{
    EN_serverError_t valid_account_or_err = isValidAccount(&transData->cardHolderData);
    if (valid_account_or_err == ACCOUNT_NOT_FOUND)
        return DECLINED_STOLEN_CARD;
    EN_serverError_t valid_amount_or_err = isAmountAvailable(&transData->terminalData);
    if (valid_amount_or_err == LOW_BALANCE)
        return DECLINED_INSUFFICIENT_FUND;
    EN_serverError_t saved_transaction_or_err = saveTransaction(transData);
    if (saved_transaction_or_err == INTERNAL_SERVER_ERROR)
        return INTERNAL_SERVER_ERROR;
    return OK;
}
```
### appStart

💳 Sale Full Cycle 💰

[AppStart.webm](https://user-images.githubusercontent.com/7840989/195393019-695de4df-6a69-44fe-8cf2-5f6bf2b2ed2e.webm)

## User Stories

### Transaction approved user story

[Transaction approved user story.webm](https://user-images.githubusercontent.com/7840989/195398616-37a4a9c4-5fac-4447-95d2-9d72b09b8c86.webm)

## Exceed the maximum amount user story

[Exceed the maximum amount user story.webm](https://user-images.githubusercontent.com/7840989/195399609-423e0dde-6aae-407e-bdfe-5495a9d64fa4.webm)

## Insufficient fund user story

[Insufficient fund user story.webm](https://user-images.githubusercontent.com/7840989/195400286-7207fddb-6ca8-458a-ba7b-c2cd9c710931.webm)

## Expired card user story

[Expired card user story.webm](https://user-images.githubusercontent.com/7840989/195400705-2ce3b7bd-2b92-4688-9de6-712720dae9a9.webm)

## Invalid card user story

[Invalid card user story.webm](https://user-images.githubusercontent.com/7840989/195402023-c97f4083-2823-45f1-b0af-92f6a82c4c03.webm)

- I assume a black list implementation that contains a list of account
numbers to match against incoming operations to block undesired access.

- note: black list is not implemented, the input card that has the pan of
"0000000000000000" is considered stolen and not present in the accounts database.

### Folder Structure

![Folder Structure](https://github.com/solomancode/fwd_project_1_payment_app/blob/master/deliver/screenshots/folder_structure.png)

