#include "./console.h"

void print_int(int n) {
    printf("%d\n", n);
}

void print_key_value(char_t key[], char_t value[]) { 
    printf("\n%s %s: %s %s\n", ANSI_BLUE, key, ANSI_RESET, value);
}

void print_title(char_t title[]) {
    printf("\n%s %s %s\n", ANSI_BLUE, title, ANSI_RESET);
}

void print_error(char_t error[]) {
    printf("\n❌ %s %s %s\n", ANSI_RED, error, ANSI_RESET);
}

void print_date_dd_mm_yyyy(int day, int month, int year) {
    printf("📅 %d/%d/%d\n", day, month, year);
}

void print_date_mm_yyyy(int month, int year) {
    printf("📅 %d/%d\n",  month, year);
}

void print_ok(char_t ok[]) {
    printf("\n✅ %s %s %s\n", ANSI_GREEN, ok, ANSI_RESET);
}

void print_ordered_list(char_t **list, unsigned int items_count, unsigned int item_size) {
    for (size_t i = 0; i < items_count; i++)
    {
        printf("%zu. %s\n", i + 1, list[i]);
    }
    printf("\n");
}