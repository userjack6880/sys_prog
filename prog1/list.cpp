/*
 * Student Name:        John Bradley
 * Student NetID:       jeb446
 * Compiler Used:       g++
 * Program Description:
 *	program 1 - C++ Version
 *	takes in a series of strings from the user, then outputs them in reverse order
 */

#include <iostream>
#include <cstring>

#include "list.hpp"

using namespace std;

static node *head = NULL;
static node *last = NULL;

/* checks to see if linked list is empty
 * if the head points nowhere, then there is no list */
bool ll_isEmpty() {
	return head == NULL;
}

/* returns the length of the list */
int ll_length() {
	int length = 0;
	node *current;

	/* count nodes until the current node points to -nothing- */
	for (current = head; current != NULL; current = current->next) {
		length++;
	}

	return length;
}

/* insert data into the linked list at the end */
void ll_insert(char *str) {
	/* create a new node first */
	node *tempNode = new node;
	if (tempNode == NULL) {
		perror("could not allocate memory");
		exit(EXIT_FAILURE);
	}

	/* make the string pointer of the node = the pointer of the string passed in */
	tempNode->str = str;

	/* if the list is empty, this pointer is now the head and last pointer */
	if (ll_isEmpty() == true) {
		head = tempNode;
    last = tempNode;            /* we assume this is the only node in the list now */
	}
	/* if it isn't empty, then this needs to go at the end of the list, so let's link up */
	else {
		tempNode->prev = last;       /* set the last node as this node's previous */
		last->next = tempNode;      /* and set the last node's next as this node */

		last = tempNode;            /* this is going to now be the last node */
	}
}

/* pop the last node off the DLL and return it */
void ll_pop() {
	/* we can't do this if there's nothing */
	if (ll_isEmpty() == true) {
		perror("no nodes in list");
		exit(EXIT_FAILURE);
	}

	/* get the string of the last node */
  cout << last->str << "\n";

	/* move the last node up one and then free up the memory */
	if (head->next == NULL) {
		/* only node in the list */
		delete head->str;
		delete head;
		head = NULL;
		last = NULL;
	}
	else {
		last = last->prev;
		delete last->next->str;
		delete last->next;
		last->next = NULL;
	}
}


