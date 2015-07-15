/*Program name:     mfind
 *Author:           Mattias Fridsén
 *e-mail:           c13mfn@cs.umu.se
 *Purpose:          A basic search function that searches for files with names
 *                  matching a given search phrase.
 *Date:             2014-10-27
 */

#ifndef _MFIND_H
#define _MFIND_H

#include <ctype.h>
#include <dirent.h>
#include <errno.h>
#include <getopt.h>
#include <limits.h>
#include <pthread.h>
#include <stdarg.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#include "mlist.h"
#include "sfunc.h"
#include "shared.h"

/*A struct containing the arguments for search(). This is needed because
 *pthread_create() takes a pointer to the argument(s) of the thread function.
 */
struct search_args
{
    int active_threads;
    char *type;
    char *phrase;
    struct listnode **first_node_ptr;
};

/*Parses program options. */
int get_options(int argc, char *argv[], char *type_ptr);

/*Calls search_function while the list of paths is not empty. */
void *thread_function(void *arguments);

#endif
