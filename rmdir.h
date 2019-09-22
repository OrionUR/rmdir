////////////////////////////////////////////////////////////
///
/// @file       rmdir.h
/// @brief      head file of rmdir
///
////////////////////////////////////////////////////////////

#ifndef RMDIR_H
#define RMDIR_H

/** define a error type
**/
typedef enum ERROR_TYPE
{
    EDEL_OK = 0,
    EDEL_OPEN_CLOSE,
    EDEL_FOLDER_FILE,
    EDEL_PATH,
} ERROR_T;

/** delete folder
 * 
 * @param: const char *path:
 *                  the folder which is going to delete
 * @return: ERROR_T:
 *                  'EDEL_OK' for success, else for failure
**/
ERROR_T delete_folder(const char *path);

#endif // RMDIR_H