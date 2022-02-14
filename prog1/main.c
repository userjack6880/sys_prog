/*
 * Student Name:        John Bradley
 * Student NetID:       jeb446
 * Compiler Used:       gcc
 * Program Description:
 *	program 1 - C version
 *	takes in a series of strings from the user, then outputs them in reverse order
 */

/* includes */
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>

#include "list.h"

/* definitions */
#define BUFF_SIZE 100

int main () {
	printf("This is a program that reads strings input until STOP is entered. Please input below.\n\n");

	/* the prompt loop! */
	while (true) {
		char *inbuf = malloc(BUFF_SIZE * sizeof(char));

		/* get text input, then look for STOP */
		scanf("%s", inbuf);

		/* if STOP, then break the loop, and go to the next step */
		if (strcmp("STOP",inbuf) == 0) {
			break;
		}

		/* now, let's put it in the list */
		ll_insert(inbuf);
	}

	printf("Thank you for your input. Displaying data in reverse.\n\n");

	while (ll_isEmpty() == false) {
		ll_pop();
	}

	exit(EXIT_SUCCESS);
}
