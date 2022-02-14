/*
 * Student Name:        John Bradley
 * Student NetID:       jeb446
 * Compiler Used:       g++
 * Program Description:
 *	program 1 - C++ version
 *	takes in a series of strings from the user, then outputs them in reverse order
 */

/* includes */
#include <iostream>
#include <cstring>

#include "list.hpp"

using namespace std;

/* definitions */
#define BUFF_SIZE 100

int main () {
	cout << "This is a program that reads strings input until STOP is entered. Please input below.\n\n";

	/* the prompt loop! */
	while (true) {
		char *inbuf = new char;

		/* get text input, then look for STOP */
		cin >> inbuf;

		/* if STOP, then break the loop, and go to the next step */
		if (strcmp("STOP",inbuf) == 0) {
			break;
		}

		/* now, let's put it in the list */
		ll_insert(inbuf);
	}

	cout << "Thank you for your input. Displaying data in reverse.\n\n";

	while (ll_isEmpty() == false) {
		ll_pop();
	}

	exit(EXIT_SUCCESS);
}
