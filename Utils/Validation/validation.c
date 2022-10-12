#include "validation.h"

bool validate_input_min_max(char_t input[], uint8_t min, uint8_t max)
{
    size_t len = strlen(input);
    return len >= min && len <= max;
}