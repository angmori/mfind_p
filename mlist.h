/*Function name:    mlist
 *Author:           Mattias Fridsén
 *e-mail:           c13mfn@cs.umu.se
 *Purpose:          A linked list containing pointers to strings.
 *                  This is the header for mlist.
 *Date:             2014-10-27
 */

#ifndef _MLIST_H
#define _MLIST_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "shared.h"

/*A listnode contains two types of data:
 *                  -str, a pointer to a string.
 *                  -next, a pointer to another struct listnode_t.
 */
struct listnode
{
    char *str;
    struct listnode *next;
};

/*Creates a new node from a string and returns the node. */
struct listnode* create_node(char *str);

/*Inserts a node into a linked list, at the end of the list. */
void insert_node(struct listnode **first_node_ptr, struct listnode *new_node);

/*Returns the first node in a list and removes it from the list. */
struct listnode* pop(struct listnode **first_node_ptr);

/*Checks whether first_node is null. If such is the case, waits until no
 *searches are running, then repeats the check to make sure the program is
 *finished.
 */
int is_empty(struct listnode *first_node);

 /*Prints the strings of all the nodes in a list. */
void print_nodes(struct listnode **first_node);

/*Removes all nodes in a list. */
void free_nodes(struct listnode **first_node);

#endif
