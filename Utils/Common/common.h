#ifndef STRING_H
#define STRING_H

#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stdbool.h>
#include <stdio.h>

typedef uint8_t char_t;

int str_to_int(char_t str[]);
float str_to_float(char_t str[]);
void float_to_string(float value, uint8_t frac, char_t out[]);

void clear_array(char arr[], int size);

size_t string_len(char_t str[]);
void string_copy(char_t dest[], const char_t source[]);
int string_slice_find_char(int start, char_t target, char_t *source, char_t *slice);

#endif