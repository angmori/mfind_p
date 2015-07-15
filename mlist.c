/*Function name:    mlist
 *Author:           Mattias Fridsén
 *e-mail:           c13mfn@cs.umu.se
 *Purpose:          A linked list containing pointers to strings.
 *Date:             2014-10-27
 */

#include "mlist.h"

/*Creates a new node from a string and returns the node. */
struct listnode* create_node(char *str)
{
    if(!str) {
        fprintf(stderr, "Error creating node.\n");
        return NULL;
    }

    unsigned int strln = strlen(str);

    struct listnode *tmp_node = malloc(sizeof(struct listnode));
    tmp_node->str = malloc((strln + 1) * sizeof(char));

    strcpy(tmp_node->str, str);
    tmp_node->next = NULL;

    return tmp_node;
}

/*Inserts a node into a linked list. */
void insert_node(struct listnode **first_node_ptr, struct listnode *new_node)
{
    pthread_mutex_lock(&mutex_insert);
    if(!(*first_node_ptr)) {
        *first_node_ptr = new_node;
        pthread_mutex_unlock(&mutex_insert);
    }
    else if(!(*first_node_ptr)->next) {
        (*first_node_ptr)->next = new_node;
        pthread_mutex_unlock(&mutex_insert);
    }
    else
    {
        struct listnode *next_node = (*first_node_ptr)->next;
        struct listnode *tmp_node = NULL;
        while(next_node) {
            tmp_node = next_node;
            next_node = tmp_node->next;
        }
        tmp_node->next = new_node;
        pthread_mutex_unlock(&mutex_insert);
    }
}

/*Returns the first node in a list and removes it from the list. */
struct listnode* pop(struct listnode **first_node_ptr)
{
    pthread_mutex_lock(&mutex_pop);
    if(!(*first_node_ptr)) {
        fprintf(stderr, "List head is null.\n");
        pthread_mutex_unlock(&mutex_pop);
        return NULL;
    } else {
        struct listnode *old_first_node = *first_node_ptr;
        *first_node_ptr = (*first_node_ptr)->next;
        pthread_mutex_unlock(&mutex_pop);
        return old_first_node;
    }
}

 /*Prints the strings of all the nodes in a list.  */
void print_nodes(struct listnode **first_node)
{
    fprintf(stdout, "Printing all nodes... \n");
    if(!(*first_node))
    {
        fprintf(stderr, "No nodes in list.\n");
    } else {
        fprintf(stdout, "%s\n", (*first_node)->str);

        struct listnode *next_node = (*first_node)->next;

        while(next_node)
        {
            fprintf(stdout, "%s\n", next_node->str);
            next_node = next_node->next;
        }
    }
}

/*Removes all nodes in a list. */
void free_nodes(struct listnode **first_node)
{
    if(!(*first_node)) {
        fprintf(stderr, "Head node is null.\n");
    } else {
        struct listnode *current_node = *first_node;
        struct listnode *next_node = current_node->next;

        while(next_node) {
            free(current_node->str);
            free(current_node);
            current_node = next_node;
            next_node = current_node->next;
        }
        free(current_node->str);
        free(current_node);

        *first_node = NULL;
    }
}
