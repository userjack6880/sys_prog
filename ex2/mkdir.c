/*
 * Student Name:        John Bradley
 * Student NetID:       jeb446
 * Compiler Used:       gcc
 * Program Description:
 *	program creates a directory structure for file associated with the exercises, assignments, and tests
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#include "mkdir.h"

void mkdir_3183 (char *dirname) {
	struct stat *dirstat = malloc(sizeof(struct stat));

	/* if the directory doesn't exist, create it */
	if (stat(dirname,dirstat) == -1 ) {
		if (mkdir(dirname, 0750) != 0) {
			perror("could not make directory: quitting\n");
			exit(EXIT_FAILURE);
		}
	}

	/* free the memory up now */
	free(dirstat);
}

void mkstructure (char *base, int sub) {
	char *buff = malloc(128 * sizeof(char));
	/* make the base directory within the root */
	mkdir_3183(base);

	/* move into the base directory */
	getcwd(buff,128);
	sprintf(buff, "%s/%s", buff, base);
	if (chdir(buff) != 0) {
		perror("could not change directory: quitting\n");
		exit(EXIT_FAILURE);
	}

	/* no longer needed */
	free(buff);

	/* truncate base for the loop */
	char *trunc = strndup(base,4);

	for (int i = 1; i < sub+1; i++) {
		/* generate the directory name */
		sprintf(trunc, "%s%i", trunc, i);
		/*  make the subdirectory */
		mkdir_3183(trunc);

		/* restore the truncated base */
		trunc = strndup(trunc,4);
	}

	/* free the memory up now */
	free(trunc);

	/* move back down to the root */
	if (chdir("..") != 0) {
		perror("could not change directory: quitting\n");
		exit(EXIT_FAILURE);
	}
}
