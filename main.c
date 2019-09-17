// '#define _DEFAULT_SOURCE' to get definitions
// that would normally be provided by default
// to get definition of 'DT_DIR'
#define _DEFAULT_SOURCE
#include "remove_folder.h"

/** check argument valid
 * 
 * @param: int argc,   number of argument 
 * @param: const char* argv,   argument vector
 * @return: int,   0 for success, else for failure
**/
static int check_arg(int argc, char* argv[])
{
    int argv_length;

    if (argc != 2)
    {
        printf("Useage: %s <dir>\n", argv[0]);
        return -1;
    }

    argv_length = strlen(argv[1]);
    if (argv_length > MAXIMUM_PATH_LENGTH)
    {
        printf("<dir> length is too long.\n");
        return -1;
    }

    return 0;
}

int main(int argc, char *argv[])
{
    DIR *dir;
    struct dirent *entry;
    char current_path[MAXIMUM_PATH_LENGTH];
    int return_val, current_path_length, path_depth_level;
    long num_of_directory = 0, num_of_file = 0;
    
    if ((return_val = check_arg(argc, argv)) != 0)
        return -1;

    strncpy(current_path, argv[1], MAXIMUM_PATH_LENGTH);
    current_path_length = strlen(current_path);
    if (current_path_length > 0)
        if (current_path[current_path_length-1] != '/')
            if ((return_val = merge_string(current_path, "/")) != 0)
                return -1;
    path_depth_level = 1;
    
    
    if (NULL == (dir = opendir(current_path)))
    {
        perror("opendir() error");
        return -1;
    }
    else
    {
        while (path_depth_level != 0)
        {
            // if folder is empty
            // then delete folder and return to the previous level
            if (NULL == (entry = readdir(dir)))
            {
                return_val = delete_folder(dir, current_path);
                if (return_val != 0)
                {
                    printf("Fail to delete folder.\n");
                    return -1;
                }
                else
                {
                    path_depth_level--;
                    num_of_directory++;
                }
            }
            else
            {
                // if this is a folder, then enter this folder
                if (entry->d_type == DT_DIR)
                {
                    return_val = enter_folder(dir, current_path, entry->d_name);
                    if (return_val == -2)
                        continue;
                    else if (return_val == -1)
                    {
                        printf("Fail to enter folder.\n");
                        return -1;
                    }
                    else
                        path_depth_level++;
                }
                // if this is not a folder, then delete it
                else
                {
                    return_val = delete_file(dir, current_path, entry->d_name);
                    if (return_val != 0)
                    {
                        printf("Fail to delete file.\n");
                        return -1;
                    }
                    else
                        num_of_file++;
                }
            }
        }
        
        if ((return_val = closedir(dir)) != 0)
        {
            perror("closedir() error");
            return -1;
        }
    }
    
    printf("\nDone!\nTotal %ld directories, %ld files have been deleted.\n",
            num_of_directory, num_of_file);
    
    return 0;
}
