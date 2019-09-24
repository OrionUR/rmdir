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

#define ROOT_FOLDER                 "mkdir ./test_folder/"
#define SUB_FOLDER      ROOT_FOLDER     "sub/"
#define SSUB_FOLDER     SUB_FOLDER          "ssub/"
#define SUB_FOLDER2     ROOT_FOLDER     "sub2/"
#define SSUB_FOLDER2    SUB_FOLDER2         "ssub2/"
#define FILEA               "touch ./test_folder/filea"
#define FILEB               "touch ./test_folder/sub2/fileb.txt"
#define DELETE_TEST_FOLDER       "rm -rf ./test/"
#define ERROR_FILE          "./test_folder/abcde"
#define ERROR_FOLDER        "./abcde/"
#define CORRECT_FILE        "./test_folder//sub2/fileb.txt"
#define CORRECT_FOLDER      "./test_folder/"

#define MAXIMUM_TEST_STRING_LENGTH      4096
#define MAXIMUM_STRING_LENGTH   MAXIMUM_TEST_STRING_LENGTH+1

int initial_test()
{
    FILE *fp;

    if ((0 != system(ROOT_FOLDER))
    ||  (0 != system(SUB_FOLDER))
    ||  (0 != system(SSUB_FOLDER))
    ||  (0 != system(SUB_FOLDER2))
    ||  (0 != system(SSUB_FOLDER2))
    ||  (0 != system(FILEA))
    ||  (0 != system(FILEB)))
    {
        return -1;
    }

    if (NULL == (fp = fopen("./test_folder/sub2/fileb.txt", "w")))
    {
        return -1;
    }
    fputs("This is text file with content", fp);
    if (0 != fclose(fp))
    {
        return -1;
    }

    return 0;
}

int end_test()
{
    if (0 != system(DELETE_TEST_FOLDER))
    {
        return -1;
    }

    return 0;
}

int main()
{
    char path_over_length[MAXIMUM_STRING_LENGTH];
    // fill full any valid character into path_over_length
    memset(path_over_length, 'a', sizeof(path_over_length));

    // initialize
    printf("initializing...\n");
    if (0 != initial_test())
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

    // test path is correct file name
    printf("testing path is correct file name...\n");
    if (EDEL_OK != delete_folder(CORRECT_FILE))
    {
        printf("test error\n");
        goto error_occur;
    }
    printf("path is correct file name......ok\n\n");

    // test path is correct folder name
    printf("testing path is correct folder name...\n");
    if (EDEL_OK != delete_folder(CORRECT_FOLDER))
    {
        printf("test error\n");
        goto error_occur;
    }
    printf("path is correct folder name......ok\n\n");
    printf("Done!!!\n");

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