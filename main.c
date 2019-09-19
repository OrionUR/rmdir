////////////////////////////////////////////////////////////
///
/// @file       main.C
/// @brief      main file
///
////////////////////////////////////////////////////////////

#include <stdio.h>
#include "remove_folder.h"

int main(int argc, char *argv[])
{
    if (2 != argc)
    {
        printf("Useage: %s <dir>\n", argv[0]);
        return -1;
    }

    switch (delete_folder(argv[1]))
    {
        case EDEL_OVER_LENGTH:
                printf("<dir> length is too long.\n");
                return -1;
        case EDEL_OPEN:
                perror("opendir() error");
                return -1;
        case EDEL_CLOSE:
                perror("closedir() error");
                return -1;
        case EDEL_FOLDER:
                perror("rmdir() error");
                return -1;
        case EDEL_FILE:
                perror("remove() error");
                return -1;
        case EDEL_DIR:
                printf("<dir> is not valid.\n");
                return -1;
        default:
                return 0;
    }
}
