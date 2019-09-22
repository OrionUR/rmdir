////////////////////////////////////////////////////////////
///
/// @file       main.c
/// @brief      main file
///
////////////////////////////////////////////////////////////

#include <stdio.h>
#include <string.h>
#include <errno.h>
#include "rmdir.h"

int main(int argc, char *argv[])
{
    if (2 != argc)
    {
        printf("Useage: %s <dir>\n", argv[0]);
        return -1;
    }

    switch (delete_folder(argv[1]))
    {
        case EDEL_OPEN_CLOSE:
                printf("opendir() or closedir() error: %s\n", strerror(errno));
                return -1;
        case EDEL_FOLDER_FILE:
                printf("remove() or rmdir() error: %s\n", strerror(errno));
                return -1;
        case EDEL_PATH:
                printf("<dir> error.\n");
                return -1;
        default:
                return 0;
    }
}
