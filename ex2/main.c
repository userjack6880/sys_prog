/*
 * Student Name:        John Bradley
 * Student NetID:       jeb446
 * Compiler Used:       gcc
 * Program Description:
 * 	program creates a directory structure for files associated with the exercises, assignments, and tests
 */

/* includes */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#include "mkdir.h"

int main (int argc, char **argv) {

	/* default values */
	char rootdir[] = "sysProg";

	/* argument handling */
	if (argc == 0) {
		perror("unexpected error\n");
		exit(EXIT_FAILURE);
	}
	if (argc == 2) {
		strcpy(rootdir, argv[1]);
	}
	if (argc > 2) {
		printf("Too many arguments! Exiting.\n");
		exit(EXIT_SUCCESS);  /* this will exit cleanly... */
	}

	/* set umask - this allows us to set all permission bits */
	umask(0000);

	/* root directory */
	/* check for directory status */
	mkdir_3183(rootdir);

	/* now move into this root directory */
	char *buff = malloc(128 * sizeof(char));
	getcwd(buff,128);
	sprintf(buff, "%s/%s", buff, rootdir);
	if (chdir(buff) != 0) {
		perror("could not change directory: quitting\n");
		exit(EXIT_FAILURE);
	}
	
	/* we can free the buffer up, we don't need it */
	free(buff);

	/* test directories */
	mkstructure("test",3);

	/* program directories */
	mkstructure("program",5);

	/* exercise directories */
	mkstructure("exercise",10);

	exit(EXIT_SUCCESS);
}
