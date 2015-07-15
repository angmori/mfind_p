/*Program name:     mfind
 *Author:           Mattias Fridsén
 *e-mail:           c13mfn@cs.umu.se
 *Purpose:          A basic search function that searches for files with names
 *                  matching a given search phrase.
 *Date:             2014-10-27
 */

#include "mfind.h"

#define working 1
#define waiting 0

pthread_mutex_t mutex_pop = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mutex_insert = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mutex_loop = PTHREAD_MUTEX_INITIALIZER;

/*Compile the program by navigating to the correct directory and typing 'make'.
 *Call it using the syntax
 *  ./mfind [-p nrthreads] [-t type] path1 [path2 ...] phrase
 *where type can be either d, f or l, corresponding to the type of file the
 *user is looking for (i. e. directory, (regular) file or (symbolic) link).
 *The program algorithm is as follows:
 *  -Get options.
 *  -Add given paths to the shared list of paths (directories to search).
 *  -Create the desired amount of threads (default 1, the main thread).
 *  -Main thread performs same function as all other threads which is:
 *      -While the list is not empty nor searches are running:
 *          -Pop the list.
 *          -Call search_function with the pop:ed element as path.
 *          -Free the pop:ed element.
 *      -Print amount of searches performed.
 *  -Join all threads and finish.
 */

int main(int argc, char* argv[])
{
    char type = 'a';
    int nrthr = get_options(argc, argv, &type);
    pthread_t threads[nrthr];
    extern int optind;
    extern char *optarg;

    struct listnode *first_node = NULL;
    struct search_args *args = malloc(sizeof(struct search_args));

    args->active_threads = nrthr;
    args->phrase = argv[argc - 1];
    args->type = &type;
    args->first_node_ptr = &first_node;

    chdir("/home");

    //Add all the paths to the list of search paths.
    for (int i = optind; i <= argc - 2; i++) {
        struct listnode *tmp_node = create_node(argv[i]);
        insert_node(&first_node, tmp_node);
    }

    //Create threads.
    for (int i = 0; i < nrthr - 1; i++) {
        pthread_create(&threads[i], NULL, thread_function, (void *)args);
    }

    //Main thread work.
    thread_function((void *)args);

    //Join threads.
    for (int i = 0; i < nrthr - 1; i++) {
        pthread_join(threads[i], NULL);
    }

    free(args);

    return 0;
}

/*Parses the options set by the user. */
int get_options(int argc, char *argv[], char *type_ptr)
{
    unsigned int nrthr = 1; //Default amount of threads for the program is 1.
    int option;

    while ((option = getopt(argc, argv,"t:p:")) != -1) {
		switch (option) {
			case 't': //The desired type of file to search for.
				*type_ptr = *optarg;
				#ifdef EBUG
				fprintf(stdout, "Type of file: %c\n", *type_ptr);
				#endif // EBUG
				break;

            case 'p': //The amount of threads to use in the search.
                nrthr = atoi(optarg);
                if (nrthr < 1) {
                    fprintf(stderr, "./mfind: Faulty argument to -p.\n");
                    exit(EXIT_FAILURE);
                }
                #ifdef EBUG
				fprintf(stdout, "Threads: %d\n", nrthr);
				#endif // EBUG
                break;

			default:
			perror("./mfind: Faulty option.\n");
            exit(EXIT_FAILURE);
		}
	}

    return nrthr;
}

/*The task each thread, including the main thread, will perform. */
void *thread_function(void *arguments)
{
    struct search_args *args = arguments;
    struct listnode *this_node = NULL;
    int loop_cntr = 0;
    int current_state = working;
    int previous_state = working;

    while(args->active_threads > 0)
    {
        pthread_mutex_lock(&mutex_loop);

        if((*args->first_node_ptr)) { //If there is anything in the list.
            this_node = pop(args->first_node_ptr);
            current_state = working;
        } else {
            current_state = waiting;
        }

        if (current_state != previous_state) {
            if (current_state == working) {
                args->active_threads++;
            } else {
                args->active_threads--;
            }
        }

        previous_state = current_state;

        pthread_mutex_unlock(&mutex_loop);

        if (current_state == working) {
            search(this_node->str, args->phrase, *(args->type),
                                                args->first_node_ptr);
            free(this_node->str);
            free(this_node);
            loop_cntr++;
        }
    }

    fprintf(stdout, "Thread: %ld Searches: %d\n", pthread_self(), loop_cntr);
    return NULL;
}
