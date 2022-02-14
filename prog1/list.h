/*
 * Student Name:        John Bradley
 * Student NetID:       jeb446
 * Compiler Used:       gcc
 * Program Description:
 *	program 1 - C version
 *	takes in a series of strings from the user, then outputs them in reverse order
 */

#ifndef DOUBLE_LINK_LIST
#define DOUBLE_LINK_LIST

/* declare the node structure, the foundation of this linked list */
typedef struct node {
	/* data stored in the node */
	char *str;

	/* the linked bit of a linked list */
	struct node *next;
	struct node *prev;
} node;

/* declare a few things so we can know and do stuff with the list */

/* know stuff */
bool ll_isEmpty();
int ll_length();

/* do stuff */
void ll_insert(char *str);
void ll_pop();

#endif
