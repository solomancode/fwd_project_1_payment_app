#ifndef FILESYSTEM_H
#define FILESYSTEM_H

#include "../Common/common.h"

typedef enum
{
    OPEN_FILE_SUCCESS,
    OPEN_FILE_ERROR
} EnumOpenFileResult;

EnumOpenFileResult read_file(char_t *path, char_t *out);
EnumOpenFileResult write_file(char_t *path, char_t *data);
#endif