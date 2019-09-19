////////////////////////////////////////////////////////////
///
/// @file       remove_folder.h
/// @brief      head file of remove folder
///
////////////////////////////////////////////////////////////

#ifndef REMOVE_FOLDER_H
#define REMOVE_FOLDER_H

/** define a error type
**/
typedef enum ERROR_TYPE
{
    EDEL_OK = 0,
    EDEL_OPEN,
    EDEL_CLOSE,
    EDEL_FOLDER,
    EDEL_FILE,
    EDEL_OVER_LENGTH,
    EDEL_DIR
} ERROR_T;

/** delete folder
 * 
 * @param: const char *path:
 *                  the folder which is going to delete
 * @return: ERROR_T:
 *                  'EDEL_OK' for success, else for failure
**/
ERROR_T delete_folder(const char *path);

#endif // REMOVE_FOLDER_H