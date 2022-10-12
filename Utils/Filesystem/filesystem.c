#include "./filesystem.h"

EnumOpenFileResult read_file(char_t *path, char_t *out)
{
    FILE *fd;
    fd = fopen(path, "r");
    if (fd == NULL) {
        return OPEN_FILE_ERROR;
    }
    uint32_t i = 0;
    char ch;
    do
    {
        ch = fgetc(fd);
        out[i] = ch;
        i++;
    } while (ch != EOF);
    out[i - 1] = '\0';
    fclose(fd);
    return OPEN_FILE_SUCCESS;
}

EnumOpenFileResult write_file(char_t *path, char_t *data)
{
    FILE *fd;
    fd = fopen(path, "w");
    if (fd == NULL) {
        return OPEN_FILE_ERROR;
    }
    int x = fprintf(fd, "%s", data);
    printf("Write data to disk: %s (%d)\n", path, x);
    fclose(fd);
    return OPEN_FILE_SUCCESS;
}