////////////////////////////////////////////////////////////
///
/// @file       rmdir_test.c
/// @brief      test remove_folder
///
////////////////////////////////////////////////////////////

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "rmdir.h"

// define tree of test folders and files
#define ROOT_FOLDER                 "./test_folder/"
#define SUB_FOLDER      ROOT_FOLDER         "sub/"
#define SSUB_FOLDER     SUB_FOLDER              "ssub/"
#define SUB_FOLDER2     ROOT_FOLDER         "sub2/"
#define SSUB_FOLDER2    SUB_FOLDER2             "ssub2/"
#define EMPTY_FILE      ROOT_FOLDER         "empty_file"
#define CONTENT_FILE    SUB_FOLDER2             "content_file.txt"
// define error folder and file
#define ERROR_FILE      ROOT_FOLDER         "abcde"
#define ERROR_FOLDER                "./abcde/"

#define MAXIMUM_TEST_STRING_LENGTH      4096
#define MAXIMUM_STRING_LENGTH   MAXIMUM_TEST_STRING_LENGTH+1

/** create folder
 * 
 * @param: const char *path:
 *                  the path of folder which is going to create
 * @return: bool:
 *                  true for success, false for failure
**/
bool create_folder(const char *path)
{
    const char  *create_folder_command = "mkdir";
    char execute_command[MAXIMUM_STRING_LENGTH];

    strncpy(execute_command, create_folder_command, 
            sizeof(create_folder_command));
    strncat(execute_command, " ", 1);
    strncat(execute_command, path, strlen(path));
    if (0 != system(execute_command))
    {
        return false;
    }

    return true;
}

/** create file
 * 
 * @param: const char *path:
 *                  the path of file which is going to create
 * @return: bool:
 *                  true for success, false for failure
**/
bool create_file(const char *path)
{
    const char  *create_file_command = "touch";
    char execute_command[MAXIMUM_STRING_LENGTH];

    strncpy(execute_command, create_file_command, 
            sizeof(create_file_command));
    strncat(execute_command, " ", 1);
    strncat(execute_command, path, strlen(path));
    if (0 != system(execute_command))
    {
        return false;
    }

    return true;
}

/** initial the test of rmdir
 *  it will create some folders and files and make it to be a tree
 * 
 * @return: bool:
 *                  true for success, false for failure
**/
bool initial_test()
{
    FILE *fp;

    if (!(create_folder(ROOT_FOLDER))
    ||  !(create_folder(SUB_FOLDER))
    ||  !(create_folder(SSUB_FOLDER))
    ||  !(create_folder(SUB_FOLDER2))
    ||  !(create_folder(SSUB_FOLDER2))
    ||  !(create_file(EMPTY_FILE))
    ||  !(create_file(CONTENT_FILE)))
    {
        return false;
    }

    if (NULL == (fp = fopen(CONTENT_FILE, "w")))
    {
        return false;
    }
    fputs("This is text file with content", fp);
    if (0 != fclose(fp))
    {
        return false;
    }

    return true;
}

/** end the test of rmdir
 *  it will delete all the folders and files when test is failure
 * 
 * @return: bool:
 *                  true for success, false for failure
**/
bool end_test()
{
    const char *delete_folder_command = "rm -rf";
    char execute_command[MAXIMUM_STRING_LENGTH];
    
    strncpy(execute_command, delete_folder_command, 
            sizeof(delete_folder_command));
    strncat(execute_command, " ", 1);
    strncat(execute_command, ROOT_FOLDER, sizeof(ROOT_FOLDER));
    if (0 != system(execute_command))
    {
        return false;
    }

    return true;
}

int main()
{
    char path_over_length[MAXIMUM_STRING_LENGTH];
    // fill full any valid character into path_over_length
    memset(path_over_length, 'a', sizeof(path_over_length));

    // initialize
    printf("initializing...\n");
    if (!(initial_test()))
    {
        printf("initialize error\n");
        return -1;
    }
    printf("initialize test folder......ok\n\n");

    // test path is NULL
    printf("testing path is NULL...\n");
    if (EDEL_PATH != delete_folder(NULL))
    {
        printf("test error\n");
        goto error_occur;
    }
    printf("path is NULL......ok\n\n");

    // test path is empty
    printf("testing path is empty...\n");
    if (EDEL_PATH != delete_folder(""))
    {
        printf("test error\n");
        goto error_occur;
    }
    printf("path is empty......ok\n\n");

    // test path is space
    printf("testing path is space...\n");
    if (EDEL_PATH != delete_folder(" "))
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

    // test path is error file name
    printf("testing path is error file name...\n");
    if (EDEL_PATH != delete_folder(ERROR_FILE))
    {
        printf("test error\n");
        goto error_occur;
    }
    printf("path is error file name......ok\n\n");

    // test path is error folder  name
    printf("testing path is error folder name...\n");
    if (EDEL_PATH != delete_folder(ERROR_FOLDER))
    {
        printf("test error\n");
        goto error_occur;
    }
    printf("path is error folder name......ok\n\n");

    // test delete a empty file
    printf("testing delete a empty file...\n");
    if (EDEL_OK != delete_folder(EMPTY_FILE))
    {
        printf("test error\n");
        goto error_occur;
    }
    printf("delete a empty file......ok\n\n");

    // test delete a file with content
    printf("testing delete a file with content...\n");
    if (EDEL_OK != delete_folder(CONTENT_FILE))
    {
        printf("test error\n");
        goto error_occur;
    }
    printf("delete a file with content......ok\n\n");

    // test delete a empty folder
    printf("testing delete a empty folder...\n");
    if (EDEL_OK != delete_folder(SSUB_FOLDER))
    {
        printf("test error\n");
        goto error_occur;
    }
    printf("delete a empty folder......ok\n\n");

    // test delete the test folder
    printf("testing delete the test folder...\n");
    if (EDEL_OK != delete_folder(ROOT_FOLDER))
    {
        printf("test error\n");
        goto error_occur;
    }
    printf("delete the test folder......ok\n\n");
    printf("Done!!!\n");

    return 0;

// if error occur, then delete test folder
error_occur:
    if (end_test())
    {
        printf("remove test folder......ok\n");
    }
    else
    {
        printf("remove test folder error\n");
    }

    return -1;
}