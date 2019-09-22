////////////////////////////////////////////////////////////
///
/// @file       rmdir.c
/// @brief      delete the path
///             whatever it is folder or file
///
////////////////////////////////////////////////////////////

// to get definitions that would normally be provided by default
// to get definition of 'DT_DIR'
#define _DEFAULT_SOURCE
#include <stdio.h>
#include <dirent.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include "rmdir.h"

#define MAXIMUM_PATH_LENGTH         4096
#define CURRENT_FOLDER              "."
#define PREVIOUS_FOLDER             ".."

static int merge_string(char *str, const char *add_on);

/** delete folder
 * 
 * @param: const char *path:
 *                  the folder/file which is going to delete
 * @return: ERROR_T:
 *                  'EDEL_OK' for success, else for failure
**/
ERROR_T delete_folder(const char *path)
{
    DIR *dir;
    struct dirent *entry;
    struct stat s_buf;
    char current_path[MAXIMUM_PATH_LENGTH], temp[MAXIMUM_PATH_LENGTH];
    char *p;
    int path_length, path_depth_level, path_changed;

    // check path's length
    // return false when length is over 4096
    if (MAXIMUM_PATH_LENGTH <= (path_length = strlen(path)))
    {
        return EDEL_PATH;
    }
    strncpy(current_path, path, path_length);

    // getting path information
    if (0 != stat(current_path, &s_buf))
    {
        return EDEL_PATH;
    }

    // if path is file, then delete it
    if (S_ISREG(s_buf.st_mode))
    {
        if (0 != remove(current_path))
        {
            return EDEL_FOLDER_FILE;
        }
        return EDEL_OK;
    }

    // path is folder
    path_changed = 1;
    path_depth_level = 1;

    // while the folder which user enter has been delete
    // then end loop
    while (0 != path_depth_level)
    {
        // open path
        // return false when no such directory
        if (1 == path_changed)
        {
            if ((NULL == (dir = opendir(current_path))))
            {
                return EDEL_OPEN_CLOSE;
            }
            path_changed = 0;
        }

        // if folder is empty
        if (NULL == (entry = readdir(dir)))
        {
            // delete folder
            if (0 != rmdir(current_path))
            {
                return EDEL_FOLDER_FILE;
            }

            // return to the previous level
            if (NULL == (p = strrchr(current_path, '/')))
            {
                return EDEL_PATH;
            }
            *p = '\0';
            path_changed = 1;

            if (0 != closedir(dir))
            {
                return EDEL_OPEN_CLOSE;
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

                if (0 != merge_string(current_path, entry->d_name))
                {
                    return EDEL_PATH;
                }
                path_changed = 1;

                if (0 != closedir(dir))
                {
                    return EDEL_OPEN_CLOSE;
                }

                path_depth_level++;
            }
            // if this is not a folder, then delete it
            else
            {
                memset(temp, 0, MAXIMUM_PATH_LENGTH);
                path_length = strlen(current_path);
                strncpy(temp, current_path, path_length);

                if (0 != merge_string(temp, entry->d_name))
                {
                    return EDEL_PATH;
                }

                if (0 != remove(temp))
                {
                    return EDEL_FOLDER_FILE;
                }
            }
        }
    }

    return EDEL_OK;
}

/** if there is no '/' in the end of str
 *  then add it
 *  after that, merge add-on into str
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

    if ((MAXIMUM_PATH_LENGTH <= (str_length + add_on_length))
    || (1 > str_length))
    {
        return -1;
    }

    if ('/' != str[str_length - 1])
    {
        strncat(str, "/", 1);
    }
    strncat(str, add_on, add_on_length);

    return 0;
}