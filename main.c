#include "remove_folder.h"

/** check argument valid
 * 
 * @param: int argc,   number of argument 
 * @param: const char* argv,   argument vector
 * @return: int,   0 for success, else for failure
**/
static ERROR_T check_arg(int argc, char* argv[])
{
    int argv_length;

    if (2 != argc)
    {
        printf("Useage: %s <dir>\n", argv[0]);
        return EDEL_DIR;
    }

    return EDEL_OK;
}

int main(int argc, char *argv[])
{
    if (EDEL_OK != (check_arg(argc, argv)))
    {
        return -1;
    }

    if (EDEL_OK != (delete_folder(argv[1])))
    {
        printf("Delete failure.\n");
        return -1;
    }
    printf("Done!\n");

    return 0;
}
