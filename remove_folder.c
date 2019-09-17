////////////////////////////////////////////////////////////
///
/// @file       remove.C
/// @brief      c file of remove folder
///
////////////////////////////////////////////////////////////

#include "remove_folder.h"
// '#define _POSIX_SOURCE' allow to use functions
// are not part of the standard C library
// but are part of the POSIX.1 standard
// for 'opendir', 'readdir' and 'rmdir' functions
//#define _POSIX_SOURCE

static const char *s_exclusion1 = ".";
static const char *s_exclusion2 = "..";

static ERROR_T check_error(DIR *dir, int type)
{
}

/** check the string whether overlength
 * 
 * @param: const char* str,   a string
 * @return: ERROR_TYPE   return 0 for ok, else error occur
**/
static ERROR_T check_overlength(const char* str1, const char* str2)
{
    int length;

    length = strlen(str1);
}

/** enter folder
 * 
 * @param: DIR* dir,   a directory stream
 * @param: char* path,   the current path
 * @param: const char* folder_name,   the folder name which is enter in
 * @return: int,   0 for success, else for failure
**/
static int enter_folder(DIR *dir, char *path, char *folder_name)
{
    int return_val;

    if (    (strcmp(folder_name, s_exclusion1) != 0)
        &&  (strcmp(folder_name, s_exclusion2) != 0))
    {
        if ((return_val = merge_string(path, folder_name)) != 0)
            return -1;

        if ((return_val = merge_string(path, "/")) != 0)
            return -1;

        if ((return_val = closedir(dir)) != 0)
        {
            perror("closedir() error");
            return -1;
        }

        printf("%s\n", path);
        if (NULL == (dir = opendir(path)))
        {
            perror("opendir() error");
            return -1;
        }
    }
    else
        return -2;

    return 0;
}

/** delete file
 * 
 * @param: DIR* dir,   a directory stream
 * @param: char* path,   the folder which is going to delete
 * @return: int,   0 for success, else for failure
**/
int delete_folder(DIR *dir, char *path)
{
    int i;
    int return_val, current_path_length;
    char *p;

    if ((return_val = rmdir(path)) != 0)
    {
        perror("rmdir() error");
        return -1;
    }

    if ((current_path_length = strlen(path)) > 2)
    {
        p = strrchr(path, '/');
        *p = '\0';
    }
    printf("%s\n", path);

    if ((return_val = closedir(dir)) != 0)
    {
        perror("closedir() error");
        return -1;
    }

    if ((dir = opendir(path)) == NULL)
    {
        perror("opendir() error");
        return -1;
    }

    return 0;
}

/** delete file
 * 
 * @param: DIR* dir,   a directory stream
 * @param: const char* path,   the current path
 * @param: const char* file_name,   the file which is going to delete
 * @return: int,   0 for success, else for failure
**/
int delete_file(DIR *dir, const char *path, char *file_name)
{
    int return_val, path_length, file_name_length;
    char temp[MAXIMUM_PATH_LENGTH];

    memset(temp, 0, MAXIMUM_PATH_LENGTH);
    path_length = strlen(path);
    if (path_length < MAXIMUM_PATH_LENGTH)
        strncpy(temp, path, path_length);
    else
    {
        printf("path is too long.\n");
        return -1;
    }

    if ((return_val = merge_string(temp, file_name)) != 0)
        return -1;
    

    if ((return_val = remove(temp)) != 0)
    {
        perror("remove() error");
        return -1;
    }
    else
        printf("File Deleted: %s\n", temp);

    return 0;
}

/** merge string
 * 
 * @param: char* str,   a string 
 * @param: const char* add_on,   other string going to add to str
 * @return: int,   0 for success, else for failure
**/
static int merge_string(char* str, char* add_on)
{
    int str_length, add_on_length;

    str_length = strlen(str);
    add_on_length = strlen(add_on);
    if (str_length + add_on_length < MAXIMUM_PATH_LENGTH)
        strncat(str, add_on, add_on_length);
    else
    {
        printf("String's length is too long.\n");
        return -1;
    }

    return 0;
}

