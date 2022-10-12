#include "select.h"
#include <stdlib.h>
#include "../Console/console.h"

Select *select_create_empty(unsigned int items_count, unsigned int item_size)
{
    char_t **data = (char_t **)calloc(items_count, item_size);
    Select *select = (Select *)malloc(sizeof(Select));
    select->items_count = items_count;
    select->item_size = item_size;
    select->next_item_index = 0;
    select->data = data;
    return select;
}

void select_cleanup(Select *select)
{
    free(select->data);
    free(select);
};

void select_add_item(Select *select, char *label)
{
    select->data[select->next_item_index] = label;
    select->next_item_index++;
}

int select_render(Select *select)
{
    print_title("Select Option");
    print_ordered_list(select->data, select->next_item_index, select->item_size);
    int selected;
    scanf("%d", &selected);
    if (selected > select->next_item_index)
    {
        print_error("Invalid Option");
    }
    return selected;
}