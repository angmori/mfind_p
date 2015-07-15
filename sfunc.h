/*Function name:    sfunc
 *Author:           Mattias Fridsén
 *e-mail:           c13mfn@cs.umu.se
 *Purpose:          A basic search function that searches for files matching
 *                  a given search phrase.
 *                  This is the header for sfunc.
 *Date:             2014-10-27
 */

#ifndef _SFUNC_H
#define _SFUNC_H

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <dirent.h>
#include <getopt.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>

#include "mlist.h"
#include "shared.h"

/*Searches for files with the following algorithm:
 *  - Open the directory 'path'.
 *  - For every file:
 *      - Skip if it is . or .. directories.
 *      - lstat the dirent.
 *      - Create a full direcory path from the file name and path.
 *      - If the name of the file corresponds to phrase:
 *          - If searching for all types of files:
 *              - Print the full file path.
 *          - If searching for directories:
 *              - If the file is directory:
 *                  - Print the full file path.
 *          - If searching for (regular) files:
 *              - If the file is (regular) file:
 *                  - Print the full file path.
 *          - If searching for links:
 *              - If the file is link:
 *                  - Print the full file path.
 *      - If the file is a directory:
 *          - Add the directory to the list of directories.
 *  - Close the directory.
 */
void search(char *path, char *phrase, char type,
                                        struct listnode **first_node_ptr);

/*Merges the strings path and dir, with a / between. */
char* create_file_path(char *path, char *dir);
#endif
