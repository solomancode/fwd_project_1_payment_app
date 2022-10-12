#ifndef DATE_H
#define DATE_H

#include <time.h>
#include <stdlib.h>
#include "../Common/common.h"

#define DATE_YEAR_OFFSET 1900
#define DATE_YY_START 2000

typedef enum
{
    DATE_FORMAT_MM_YY,
    DATE_FORMAT_DD_MM_YYYY,
} EnumDateFormat;

typedef struct
{
    EnumDateFormat format;
    int year;
    int month;
    int day;
} Date;

typedef enum
{
    DATE_PARSE_OK,
    DATE_PARSE_FAILED
} EnumDateParseResult;

void date_get_today(Date *out);

EnumDateParseResult date_parse_date_string_mm_yy(char_t date[], Date *out);
EnumDateParseResult date_parse_date_string_dd_mm_yyyy(char_t date_str[], Date *out);

void date_to_string_mm_yy(Date *date, char_t out[]);
void date_to_string_dd_mm_yyyy(Date *date, char_t out[]);

typedef enum
{
    DATE_1_GT_DATE_2,
    DATE_2_GT_DATE_1,
    DATE_1_EQ_DATE_2
} EnumDateCmpResult;

EnumDateCmpResult date_compare_dates(Date *date1, Date *date2);

#endif