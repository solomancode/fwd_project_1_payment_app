#include "common.h"

int str_to_int(char_t str[])
{
    return atoi(str);
    int j = 0;
    char_t int_str[10] = {};
    clear_array(int_str,10);
    bool pad = true;
    for (size_t i = 0; i < strlen(str); i++)
    {
        char_t c = str[i];
        if (pad == true && c == '0')
        {
            continue;
        }
        else
        {
            pad = false;
            int_str[j] = c;
            j++;
        }
    }
    if (str[0] == '0')
        return 0;
    return atoi(int_str);
}

float str_to_float(char_t str[])
{
    int j = 0;
    char_t int_str[10];
    bool pad = true;
    for (size_t i = 0; i < strlen(str); i++)
    {
        char_t c = str[i];
        if (pad == true && c == '0')
        {
            continue;
        }
        else
        {
            pad = false;
            int_str[j] = c;
            j++;
        }
    }
    if (str[0] == '0')
        return 0;
    return atof(int_str);
}

void clear_array(char arr[], int size)
{
    memset(arr, 0, size);
}

void string_copy(char_t dest[], const char_t source[]) {
    strcpy(dest, source);
}

size_t string_len(char_t str[]) {
    return strlen(str);
}

void float_to_string(float value, uint8_t frac, char_t out[])
{
    gcvt(value, frac, out);
}

int string_slice_find_char(int start, char_t target, char_t *source, char_t *slice)
{
    int j = 0;
    size_t i = start;
    for (; i < string_len(source); i++)
    {
        char current = source[i];
        if (current == target)
            return i;
        slice[j] = source[i];
        j++;
    }

    return i;
}
