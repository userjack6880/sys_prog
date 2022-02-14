/*
 * Student Name:	John Bradley
 * Student NetID:	jeb446
 * Compiler Used:	gcc
 * Program Description:
 * 	A program that generates a set of random numbers from three ranges and stores
 * 	them in a binary file using system calls.
 */

/* includes */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

union udt {
	int i;
	char c[4];
};

int main (int argc, char **argv) {

	/* a few defaults */
	char filename[32] = "flowData.dat";
	int numVals = 60;
	int lowVal = 5;
	int medVal = 10;
	int hiVal = 15;

	/* seed random */
	srand(time(0));

	/* command line argument order
	 * createFile <filename> <number> <low value> <middle value> <high value>
	 * <filename> - name of binary file
	 * <number> - total number of values to generate, default to 60
	 * <low value> - average value of the lower range, default to 5
	 * <middle value> - average value of the middle range, default to 10
	 * <high value> - average value of the upper range, default to 15
	 */

	if (argc == 0) {							/* this should never happen */
		perror("unexpected error");
		exit(EXIT_FAILURE);
	}
	if ((argc > 1 && argc < 3) || (argc > 3 && argc < 6) || argc > 6) {		/* invalid number of args */
		perror("invalid number of args");
		exit(EXIT_FAILURE);
	}
	if (argc >= 3) { 
		strcpy(filename, argv[1]);
		numVals = atoi(argv[2]); 
	}
	if (argc == 6) {
		lowVal = atoi(argv[3]);
		medVal = atoi(argv[4]);
		hiVal = atoi(argv[5]);
	}

	/* open/create the file */
	int fh;

	fh = open(filename,O_CREAT | O_WRONLY | O_TRUNC); 
	if (fh == -1) {
		perror("could not open file");
		exit(EXIT_FAILURE);
	}

	/* loop while all numbers haven't been written yet */
	int i = 0;
	while (i < numVals) {

		/* generate random number between 0 and 100 to determine range
		 * 	0-15 lower range
		 * 	16-84 middle range
		 * 	85-100 upper range
		 */
		int range = (rand() % (100 + 1));

		/* generate random number between -0.5 and 0.5 to create offset */
		float offset = (((float)rand()/RAND_MAX) - 0.5);

		/* use union and convert integer to 4 byte char buffer */
		union udt genInt;

		/* generate value
		 * 	1. add offset to chosen range's average
		 * 	2. multiply result by 100
		 * 	3. truncate to integer
		 */
		if (range <= 15) {
			genInt.i = (int)(100*((float)lowVal + offset));
		}
		else if (range >= 85) {
			genInt.i = (int)(100*((float)hiVal + offset));
		}
		else {
			genInt.i = (int)(100*((float)medVal + offset));
		}

		/* output only 2 bytes of char buffer */
		write(fh,genInt.c,2);

		i++;
	}

	/* close the file */
	close(fh);

	exit(EXIT_SUCCESS);
}
