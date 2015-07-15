/*Name:             shared
 *Author:           Mattias Fridsén
 *e-mail:           c13mfn@cs.umu.se
 *Purpose:          Shared variables.
 *Date:             2015-05-15
 */

#include <pthread.h>
#include <semaphore.h>

extern pthread_mutex_t mutex_pop;
extern pthread_mutex_t mutex_insert;
extern pthread_mutex_t mutex_loop;
