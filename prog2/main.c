/*
 * Student Name:	John Bradley
 * Student NetID:	jeb446
 * Compiler Used:	gcc
 * Program Description:
 *	A program that reads in a binary file containing a series of 2-byte
 *	random numbers and interprets it as high flow, low flow, or correct
 *	flow.
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

int main () {

	/* a few defaults */
	char filename[32] = "flowData.dat";
	int lowValWarn = 550;
	int lowVal = 950;
	int hiVal = 1050;
	int hiValWarn = 1450;

	/* open the file */
	int fh;

	fh = open(filename,O_RDONLY); 
	if (fh == -1) {
		perror("could not open file");
		exit(EXIT_FAILURE);
	}

	union udt buff;
	ssize_t bytesRead;
	int counter = 0;

	while (1) {
		/* read in 2 bytes at a time */
		bytesRead = read(fh,buff.c,2);

		/* if bytesRead is -1, error */
		if (bytesRead == -1) {
			perror("error reading value");
			exit(EXIT_FAILURE);
		}

		/* if bytesRead is 0, it's EOF */
		if (bytesRead == 0) {
			break;
		}

		/* evaluate the value */

		/* if within lowVal <-> hiVal, increment counter */
		if (buff.i >= lowVal && buff.i <= hiVal) {
			counter++;
		}

		/* if lower than lowVal or higher than hiVal, print alert, then reset counter */
		else {
			/* if lower than lowValWarn, alert is low pressure */
			if (buff.i <= lowValWarn) {
				printf("   ALERT! PRESSURE IS \tLOW!\t    %i \treadings since last alert\n",counter);
			}

			/* if higher than hiValWarn, alert is high pressure */
			else if (buff.i >= hiValWarn) {
				printf("   ALERT! PRESSURE IS \tHIGH!\t    %i \treadings since last alert\n",counter);
			}

			/* otherwise, just alert outside of normal */
			else {
				printf("ALERT! PRESSURE OUTSIDE OF EXPECTED VALUE\t    %i \treadings since last alert\n",counter);
			}

			/* reset counter */
			counter = 0;
		}
	}

	/* close the file */
	close(fh);

	exit(EXIT_SUCCESS);
}
