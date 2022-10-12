#include "date.h"

void date_get_today(Date *out)
{
    time_t ts = time(NULL);
    struct tm now = *localtime(&ts);
    out->year = now.tm_year + DATE_YEAR_OFFSET;
    out->month = now.tm_mon + 1;
    out->day = now.tm_mday;
    out->format = DATE_FORMAT_DD_MM_YYYY;
}

EnumDateParseResult date_parse_date_string_dd_mm_yyyy(char_t date_str[], Date *out)
{
    
    if (strlen(date_str) != 10)
    {
        return DATE_PARSE_FAILED;
    }

    char_t dd_str[3] = {date_str[0], date_str[1]};
    char_t mm_str[3] = {date_str[3], date_str[4]};
    char_t yyyy_str[5] = {date_str[6], date_str[7], date_str[8], date_str[9]};

    out->format = DATE_FORMAT_DD_MM_YYYY;
    out->year = atoi(yyyy_str);
    out->month = atoi(mm_str);
    out->day = atoi(dd_str);

    return DATE_PARSE_OK;
}

EnumDateParseResult date_parse_date_string_mm_yy(char_t date_str[], Date *out)
{
    if (strlen(date_str) != 5)
    {
        return DATE_PARSE_FAILED;
    }

    char_t mm_str[3] = {date_str[0], date_str[1]};
    char_t yy_str[3] = {date_str[3], date_str[4]};

    out->format = DATE_FORMAT_MM_YY;
    out->month = atoi(mm_str);
    out->year = atoi(yy_str) + DATE_YY_START;
    out->day = 1;

    return DATE_PARSE_OK;
}

EnumDateCmpResult date_compare_dates(Date *date1, Date *date2)
{
    int diff_years = date1->year - date2->year;
    if (diff_years > 0)
        return DATE_1_GT_DATE_2;
    if (diff_years < 0)
        return DATE_2_GT_DATE_1;
    int diff_months = date1->month - date2->month;
    if (diff_months > 0)
        return DATE_1_GT_DATE_2;
    if (diff_months < 0)
        return DATE_2_GT_DATE_1;
    if (date1->format == DATE_FORMAT_DD_MM_YYYY)
    {
        int diff_days = date1->day - date2->day;
        if (diff_days > 0)
            return DATE_1_GT_DATE_2;
        if (diff_days < 0)
            return DATE_2_GT_DATE_1;
    }
    return DATE_1_EQ_DATE_2;
};

void date_to_string_mm_yy(Date* date, char_t out[]) {
    size_t len;
    switch (date->format)
    {
    case DATE_FORMAT_MM_YY:
        len = 5;
        snprintf(out, len,"%d/%d", date->month, date->year);
        break;
    
    case DATE_FORMAT_DD_MM_YYYY:
        len = 9;
        snprintf(out, len,"%d/%d", date->month, date->year);
        break;
    }
}

void date_to_string_dd_mm_yyyy(Date* date, char_t out[]) {
    size_t len;
    switch (date->format)
    {
    case DATE_FORMAT_MM_YY:
        len = 11;
        snprintf(out, len,"1/%d/%d", date->month, date->year);
        break;
    
    case DATE_FORMAT_DD_MM_YYYY:
        len = 11;
        snprintf(out, len,"%d/%d/%d", date->day, date->month, date->year);
        break;
    }
}