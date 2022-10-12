#ifndef CONSOLE_H
#define CONSOLE_H

#include <stdio.h>
#include "../Common/common.h"

#define ANSI_GREEN "\x1b[32m"
#define ANSI_RED "\x1b[31m"
#define ANSI_BLUE "\x1b[44m"
#define ANSI_RESET "\x1b[0m"

#define green_text(TXT) ANSI_GREEN TXT ANSI_RESET
#define red_text(TXT) ANSI_RED TXT ANSI_RESET
#define blue_text(TXT) ANSI_BLUE TXT ANSI_RESET

void print_title(char_t title[]);
void print_int(int n);
void print_key_value(char_t key[], char_t value[]);
void print_date_dd_mm_yyyy(int day, int month, int year);
void print_date_mm_yy(int month, int year);
void print_error(char_t error[]);
void print_ok(char_t ok[]);
void print_ordered_list(char_t **list, unsigned int items_count, unsigned int item_size);

#endif
