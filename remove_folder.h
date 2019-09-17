////////////////////////////////////////////////////////////
///
/// @file       remove.h
/// @brief      head file of remove folder
///
////////////////////////////////////////////////////////////
#ifndef REMOVE_FOLDER_H
#define REMOVE_FOLDER_H

#include <stdio.h>
#include <dirent.h>
#include <errno.h>
#include <sys/types.h>
#include <string.h>
#include <unistd.h>

#define MAXIMUM_PATH_LENGTH        4096

/** define a error type
 * there is total 5 error types
 * start from 0
**/
typedef enum ERROR_TYPE
{
    EDEL_OK = 0,
    EDEL_OPEN,
    EDEL_CLOSE,
    EDEL_FOLDER,
    EDEL_FILE
} ERROR_T;


/** delete folder
 * 
 * @param: DIR* dir,   a directory stream
 * @param: char* path,   the folder which is going to delete
 * @return: int,   0 for success, else for failure
**/
ERROR_T delete_folder(const char* path);

#endif