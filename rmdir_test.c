////////////////////////////////////////////////////////////
///
/// @file       rmdir_test.c
/// @brief      test remove_folder
///
////////////////////////////////////////////////////////////

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "rmdir.h"

#define TEST_FOLDER                     "test_folder"
#define TEST_LEVEL                      10
#define MAXIMUM_STRING_LENGTH           6000
#define MAXIMUM_TEST_STRING_LENGTH      4096

int initial_test()
{
    char *create_folder_command = "mkdir",
         *create_file_command = "touch";
    char path[MAXIMUM_STRING_LENGTH],
         temp[MAXIMUM_STRING_LENGTH];
    char ci[3];
    int i;

    // create test folder
    memset(temp, 0, MAXIMUM_STRING_LENGTH);
    memset(path, 0, MAXIMUM_STRING_LENGTH);
    strncat(path, " ", 2);
    strncat(path, TEST_FOLDER, strlen(TEST_FOLDER));
    strncat(temp, create_folder_command, strlen(create_folder_command));
    strncat(temp, path, strlen(path));
    system(temp);

    // create 10 folders and 10 files
    // each folder include 1 file
    for (i=1; i<=TEST_LEVEL; i++)
    {
        // convert int to char
        // and add it into path
        sprintf(ci, "%d", i);
        strncat(path, "/", 2);
        strncat(path, ci, strlen(ci));

        // create folder
        memset(temp, 0, MAXIMUM_STRING_LENGTH);
        strncat(temp, create_folder_command, strlen(create_folder_command));
        strncat(temp, path, strlen(path));
        if (0 != system(temp))
        {
            return -1;
        }

        // create file
        memset(temp, 0, MAXIMUM_STRING_LENGTH);
        strncat(temp, create_file_command, strlen(create_file_command));
        strncat(temp, path, strlen(path));
        strncat(temp, "/", 2);
        strncat(temp, ci, strlen(ci));
        strncat(temp, ".txt", 5);
        if (0 != system(temp))
        {
            return -1;
        }
    }

    return 0;
}

int end_test()
{
    char *rmdir_command = "rm -rf";
    char temp[MAXIMUM_STRING_LENGTH];

    memset(temp, 0, MAXIMUM_STRING_LENGTH);
    strncat(temp, rmdir_command, strlen(rmdir_command));
    strncat(temp, " ", 2);
    strncat(temp, TEST_FOLDER, strlen(TEST_FOLDER));
    if (0 != system(temp))
    {
        return -1;
    }

    return 0;
}

int main()
{
    char *path_null = NULL,
         *path_empty = "",
         *path_space = " ",
         *path_error_folder = "./abcdef",
         *path_error_file = "./test_folder/1/2/3/4/8.txt",
         *path_correct_folder = "./test_folder",
         *path_correct_file = "./test_folder/1/2/3/3.txt";
    char path_over_length[MAXIMUM_STRING_LENGTH];
    memset(path_over_length, 50, MAXIMUM_TEST_STRING_LENGTH);

    // initialize
    printf("initializing...\n");
    if (0 != initial_test())
    {
        printf("initialize error\n");
        return -1;
    }
    system("tree ./test_folder");
    printf("initialize test folder......ok\n\n");

    // test path is NULL
    printf("testing path is NULL...\n");
    if (EDEL_PATH != delete_folder(path_null))
    {
        printf("test error\n");
        goto error_occur;
    }
    printf("path is NULL......ok\n\n");

    // test path is empty
    printf("testing path is empty...\n");
    if (EDEL_PATH != delete_folder(path_empty))
    {
        printf("test error\n");
        goto error_occur;
    }
    printf("path is empty......ok\n\n");

    // test path is space
    printf("testing path is space...\n");
    if (EDEL_PATH != delete_folder(path_space))
    {
        printf("test error\n");
        goto error_occur;
    }
    printf("path is space......ok\n\n");

    // test path over length
    printf("testing path is over length...\n");
    if (EDEL_PATH != delete_folder(path_over_length))
    {
        printf("test error\n");
        goto error_occur;
    }
    printf("path is over length......ok\n\n");

    // test path is error folder  name
    printf("testing path is error folder name...\n");
    if (EDEL_PATH != delete_folder(path_error_folder))
    {
        printf("test error\n");
        goto error_occur;
    }
    printf("path is error folder name......ok\n\n");

    // test path is error file name
    printf("testing path is error file name...\n");
    if (EDEL_PATH != delete_folder(path_error_file))
    {
        printf("test error\n");
        goto error_occur;
    }
    printf("path is error file name......ok\n\n");

    // test path is correct folder name
    printf("testing path is correct file name...\n");
    if (EDEL_OK != delete_folder(path_correct_file))
    {
        printf("test error\n");
        goto error_occur;
    }
    printf("path is correct file name......ok\n\n");

    // test path is correct folder name
    printf("testing path is correct folder name...\n");
    if (EDEL_OK != delete_folder(path_correct_folder))
    {
        printf("test error\n");
        goto error_occur;
    }
    printf("path is correct folder name......ok\n\n");
    printf("test done!!!\n");

    return 0;

// if error occur, then delete test folder
error_occur:
    if (0 != end_test())
    {
        printf("remove test folder error\n");
    }
    else
    {
        printf("remove test folder......ok\n\n");
    }

    return -1;
}