////////////////////////////////////////////////////////////
///
/// @file       remove.C
/// @brief      c file of remove folder
///
////////////////////////////////////////////////////////////

// '#define _DEFAULT_SOURCE' to get definitions
// that would normally be provided by default
// to get definition of 'DT_DIR'
#define _DEFAULT_SOURCE
#include "remove_folder.h"

static ERROR_T enter_folder(char *path, const char *folder_name);
static ERROR_T delete_empty_folder(char *path);
static ERROR_T delete_file(const char *path, const char *file_name);
static ERROR_T merge_string(char *str, const char *add_on);
static ERROR_T check_slash(const char *str);

static const char *s_exclusion1 = ".";
static const char *s_exclusion2 = "..";
static DIR *dir;

/** delete folder
 * 
 * @param: const char *path:
 *                  the folder which is going to delete
 * @return: ERROR_T:
 *                  'EDEL_OK' for success, else for failure
**/
ERROR_T delete_folder(const char *path)
{
    struct dirent *entry;
    char current_path[MAXIMUM_PATH_LENGTH];
    int path_length, path_depth_level;
    long num_of_directory = 0, num_of_file = 0;

    // check path's length
    // return false when length is over 4096
    if (MAXIMUM_PATH_LENGTH < (path_length = strlen(path)))
    {
        printf("<dir> length is too long.\n");
        return EDEL_OVER_LENGTH;
    }
    strncpy(current_path, path, path_length);
    path_depth_level = 1;

    // open path
    // return false when no such directory
    if (NULL == (dir = opendir(current_path)))
    {
        perror("opendir() error");
        return EDEL_OPEN;
    }
    // open path success
    else
    {
        // while the folder which user enter has been delete
        // then end loop
        while (0 != path_depth_level)
        {
            // if folder is empty
            // then delete folder and return to the previous level
            if (NULL == (entry = readdir(dir)))
            {
                if (EDEL_OK != (delete_empty_folder(current_path)))
                {
                    printf("Fail to delete folder.\n");
                    return EDEL_FOLDER;
                }
                path_depth_level--;
                num_of_directory++;
            }
            // folder is not empty
            else
            {
                // if this is a folder, then enter this folder
                if (DT_DIR == entry->d_type)
                {
                    switch (enter_folder(current_path, entry->d_name))
                    {
                    case EDEL_EXCLUSION:
                        continue;
                    case EDEL_OK:
                        path_depth_level++;
                        break;
                    default:
                        printf("Fail to enter folder.\n");
                        return EDEL_ENTER;
                    }
                }
                // if this is not a folder, then delete it
                else
                {
                    if (EDEL_OK != (delete_file(current_path, entry->d_name)))
                    {
                        printf("Fail to delete file.\n");
                        return EDEL_FILE;
                    }
                    num_of_file++;
                }
            }
        }

        // close path
        // return false when close path failure
        if (EDEL_OK != (closedir(dir)))
        {
            perror("closedir() error");
            return EDEL_CLOSE;
        }
    }

    // print out how many folders and files have been deleted
    printf("\nTotal %ld directories, %ld files have been deleted.\n",
           num_of_directory, num_of_file);

    return EDEL_OK;
}

/** enter folder
 * 
 * @param:  char *path:
 *                  the current path
            const char *folder_name:
                    the folder name which is going to enter
 * @return: ERROR_T:
 *                  'EDEL_OK' for success
 *                  'EDEL_EXCLUSION' for ignore
 *                  else for failure
**/
static ERROR_T enter_folder(char *path, const char *folder_name)
{
    // ignore folder name with '.' and '..'
    if (    (0 == strcmp(folder_name, s_exclusion1)) 
        &&  (0 == strcmp(folder_name, s_exclusion2)))
    {
        return EDEL_EXCLUSION;
    }

    // if end of path is not '/', then add '/'
    // return false when the length is over 4096
    if (EDEL_OK != (check_slash(path)))
    {
        if (EDEL_OK != (merge_string(path, "/")))
        {
            return EDEL_OVER_LENGTH;
        }
    }

    // add folder name to the path which is going to enter
    // return false when path's length is over 4096 after added
    if (EDEL_OK != (merge_string(path, folder_name)))
    {
        return EDEL_OVER_LENGTH;
    }

    if (EDEL_OK != (closedir(dir)))
    {
        perror("closedir() error");
        return EDEL_CLOSE;
    }

    if (NULL == (dir = opendir(path)))
    {
        perror("opendir() error");
        return EDEL_OPEN;
    }

    return EDEL_OK;
}

/** delete empty folder
 * 
 * @param:  char *path:
 *                  the folder which is going to delete
 * @return: ERROR_T:
 *                  'EDEL_OK' for success, else for failure
**/
static ERROR_T delete_empty_folder(char *path)
{
    int i;
    int path_length;
    char *p;

    // delete empty folder and print out message when delete success
    if (EDEL_OK != (rmdir(path)))
    {
        perror("rmdir() error");
        return EDEL_FOLDER;
    }
    printf("Folder deleted: %s\n", path);

    // return to previous level
    if (NULL != (p = strrchr(path, '/')))
    {
        *p = '\0';
    }

    if (EDEL_OK != (closedir(dir)))
    {
        perror("closedir() error");
        return EDEL_CLOSE;
    }

    if (NULL == (dir = opendir(path)))
    {
        perror("opendir() error");
        return EDEL_OPEN;
    }

    return EDEL_OK;
}

/** delete file
 * 
 * @param:  const char *path:
 *                  the current path
            const char *file_name:
                    the file which is going to delete
 * @return: ERROR_T:
 *                  'EDEL_OK' for success, else for failure
**/
static ERROR_T delete_file(const char *path, const char *file_name)
{
    int path_length, file_name_length;
    char temp[MAXIMUM_PATH_LENGTH];

    memset(temp, 0, MAXIMUM_PATH_LENGTH);
    // check path's length
    /** make sure it never over length, so whether check it again or not
    if (MAXIMUM_PATH_LENGTH < (path_length = strlen(path)))
    {
        printf("path is too long.\n");
        return EDEL_OVER_LENGTH;
    }
    **/
    strncpy(temp, path, path_length);

    // if end of temp path is not '/', then add '/'
    // return false when the length is over 4096
    if (EDEL_OK != (check_slash(temp)))
    {
        if (EDEL_OK != (merge_string(temp, "/")))
        {
            return EDEL_OVER_LENGTH;
        }
    }

    // add file name to the temp path which is going to delete
    // return false when path's length is over 4096 after added
    if (EDEL_OK != (merge_string(temp, file_name)))
    {
        return EDEL_OVER_LENGTH;
    }

    // delete file and print out message when delete success
    if (EDEL_OK != (remove(temp)))
    {
        perror("remove() error");
        return EDEL_FILE;
    }
    printf("File deleted: %s\n", temp);

    return EDEL_OK;
}

/** merge string
 * 
 * @param:  char *str:
 *                  a string
            const char *add_on:
                    other string going to add to str
 * @return: ERROR_T:
 *                  'EDEL_OK' for success, else for failure
**/
static ERROR_T merge_string(char *str, const char *add_on)
{
    int str_length, add_on_length;

    str_length = strlen(str);
    add_on_length = strlen(add_on);
    if (MAXIMUM_PATH_LENGTH < (str_length + add_on_length))
    {
        printf("new string's length is too long.\n");
        return EDEL_OVER_LENGTH;
    }
    strncat(str, add_on, add_on_length);

    return EDEL_OK;
}

/** check slash
 * 
 * @param:  const char *str:
 *                  a string going to check
 * @return: ERROR_T:
 *                  'EDEL_OK' for there is a slash in the end
 *                  else for not
**/
static ERROR_T check_slash(const char *str)
{
    int str_length;

    if (1 < (str_length = strlen(str)))
    {
        if ('/' == str[str_length - 1])
        {
            return EDEL_OK;
        }
        else
        {
            return EDEL_NO_SLASH;
        }
    }
}