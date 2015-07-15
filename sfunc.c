/*Function name:    sfunc
 *Author:           Mattias Fridsén
 *e-mail:           c13mfn@cs.umu.se
 *Purpose:          A basic search function that searches for files matching
 *                  a given search phrase.
 *Date:             2014-10-27
 */

#include "sfunc.h"

/*A search function that looks for phrase in path. */
void search(char *path, char *phrase, char type, struct listnode **first_node_ptr)
{
    DIR *d;
    int close;
    struct dirent *dir;
    struct stat sb;

    // Opens the directory path.
    if(!(d = opendir(path)))
    {
        #ifdef EBUG
        perror("./mfind");
        #endif // EBUG
    } else
    {
        // Reads the contents of the directory path.
        while((dir = readdir(d)))
        {
            // Skips . and .. folders, so as to avoid infinte looping.
            if((strcmp(dir->d_name, ".") == 0) ||
                        (strcmp(dir->d_name, "..") == 0))
            {
                continue;
            }

            // Creates a full path to the file.
            char *absPath = create_file_path(path, dir->d_name);

            // lstat's the current file.
            if(lstat(absPath, &sb))
            {
                #ifdef EBUG
                perror(absPath);
                continue;
                #endif // EBUG
            }

            // If the file name corresponds to the search phrase:
            if(strcmp(dir->d_name, phrase) == 0)
            {
                switch(type) {
                    // If looking for all types of files, print full file path.
                    case 'a':
                        fprintf(stdout, "%s\n", absPath);
                        break;
                    // If looking for directories, check if directory, if so,
                    // print full file path.
                    case 'd':
                        if(S_ISDIR(sb.st_mode))
                            fprintf(stdout, "%s\n", absPath);
                        break;
                    // If looking for regular files, check if regular file, if
                    // so, print full file path.
                    case 'f':
                        if(S_ISREG(sb.st_mode))
                            fprintf(stdout, "%s\n", absPath);
                        break;
                    // If looking for links, check if links, if so, print full
                    // file path.
                    case 'l':
                        if(S_ISLNK(sb.st_mode))
                            fprintf(stdout, "%s\n", absPath);
                        break;
                    // This means something has gone very wrong, as an option
                    // argument has been given that shouldn't be possible.
                    default:
                        fprintf(stderr, "%c: Unknown file type!\n", type);
                        exit(EXIT_FAILURE);
                }
            }

            // If current file is a directory, add it to the list to search.
            if(S_ISDIR(sb.st_mode) && !S_ISLNK(sb.st_mode))
            {
               struct listnode *tmp_node = create_node(absPath);
               insert_node(first_node_ptr, tmp_node);
            }

            free(absPath);
        }

        // Close directory.
        if((close = closedir(d)))
        {
            exit(EXIT_FAILURE);
        }
    }
}

/*Merges the strings path and dir, with a / between. */
char* create_file_path(char *path, char *dir)
{
    char *absPath = malloc(strlen(path) + strlen(dir) + 2);
    if(!absPath)
    {
        exit(EXIT_FAILURE);
    }

    strcpy(absPath, path);
    strcat(absPath, "/");
    strcat(absPath, dir);

    return absPath;
}
