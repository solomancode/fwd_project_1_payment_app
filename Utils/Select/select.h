#ifndef SELECT_H
#define SELECT_H

#include "../Common/common.h"

typedef struct
{
    unsigned int items_count;
    unsigned int item_size;
    unsigned int next_item_index;
    char_t **data;
} Select;

Select *select_create_empty(unsigned int items_count, unsigned int item_size);

void select_add_item(Select *select, char *label);

int select_render(Select *select);

void select_cleanup(Select *select);

#endif