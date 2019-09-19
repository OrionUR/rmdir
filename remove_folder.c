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

#include <stdio.h>
#include <dirent.h>
//#include <errno.h>
//#include <sys/types.h>
#include <string.h>
#include <unistd.h>
#include "remove_folder.h"

#define MAXIMUM_PATH_LENGTH         4096
#define CURRENT_FOLDER              "."
#define PREVIOUS_FOLDER             ".."

static int merge_string(char *str, const char *add_on);
static int complete_slash(char *str);

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
    char current_path[MAXIMUM_PATH_LENGTH], temp[MAXIMUM_PATH_LENGTH];
    char *p;
    int path_length, path_depth_level;

    // check path's length
    // return false when length is over 4096
    if (MAXIMUM_PATH_LENGTH <= (path_length = strlen(path)))
    {
        return EDEL_OVER_LENGTH;
    }
    strncpy(current_path, path, path_length);
    path_depth_level = 1;

    // open path
    // return false when no such directory
    if (NULL == (dir = opendir(current_path)))
    {
        return EDEL_OPEN;
    }

    // while the folder which user enter has been delete
    // then end loop
    while (0 != path_depth_level)
    {
        // if folder is empty
        if (NULL == (entry = readdir(dir)))
        {
            // delete folder
            if (0 != rmdir(current_path))
            {
                return EDEL_FOLDER;
            }

            // return to the previous level
            if (NULL == (p = strrchr(current_path, '/')))
            {
                return EDEL_DIR;
            }
            *p = '\0';

            if (0 != closedir(dir))
            {
                return EDEL_CLOSE;
            }

            if (NULL == (dir = opendir(current_path)))
            {
                return EDEL_OPEN;
            }
            path_depth_level--;
        }
        // if folder is not empty
        else
        {
            // if this is a folder, then enter this folder
            if (DT_DIR == entry->d_type)
            {
                // ignore folder name with ".", and ".."
                if ((0 == strcmp(entry->d_name, CURRENT_FOLDER))
                ||  (0 == strcmp(entry->d_name, PREVIOUS_FOLDER)))
                {
                    continue;
                }

                if ((0 != complete_slash(current_path))
                || (0 != merge_string(current_path, entry->d_name)))
                {
                    return EDEL_OVER_LENGTH;
                }

                if (0 != closedir(dir))
                {
                    return EDEL_CLOSE;
                }

                if (NULL == (dir = opendir(current_path)))
                {
                    return EDEL_OPEN;
                }
                path_depth_level++;
            }
            // if this is not a folder, then delete it
            else
            {
                memset(temp, 0, MAXIMUM_PATH_LENGTH);
                path_length = strlen(current_path);
                strncpy(temp, current_path, path_length);

                if ((0 != complete_slash(temp))
                || (0 != merge_string(temp, entry->d_name)))
                {
                    return EDEL_OVER_LENGTH;
                }

                if (0 != remove(temp))
                {
                    return EDEL_FILE;
                }
            }
        }
    }

    if (EDEL_OK != (closedir(dir)))
    {
        return EDEL_CLOSE;
    }

    return EDEL_OK;
}

/** merge string
 * 
 * @param:  char *str:
 *                  a string
            const char *add_on:
                    other string going to add to str
 * @return: int:
 *                  0 for success, -1 for failure
**/
static int merge_string(char *str, const char *add_on)
{
    int str_length, add_on_length;

    str_length = strlen(str);
    add_on_length = strlen(add_on);
    if (MAXIMUM_PATH_LENGTH <= (str_length + add_on_length))
    {
        return -1;
    }
    strncat(str, add_on, add_on_length);

    return 0;
}

/** complete slash
 * 
 * @param:  char *str:
 *                  a string going to check
 * @return: int:
 *                  0 for there is a slash in the end
 *                    or add slash in the end
 *                  -1 for failure
**/
static int complete_slash(char *str)
{
    int str_length;

    if (1 < (str_length = strlen(str)))
    {
        if ('/' == str[str_length - 1])
        {
            return 0;
        }
        return merge_string(str, "/");
    }
}