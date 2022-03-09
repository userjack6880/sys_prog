/*
 * Student Name:        John Bradley
 * Student NetID:       jeb446
 * Compiler Used:       gcc
 * Program Description:
 *	quiz3 program
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

int main (int argc, char *argv[]) {
	/* some initial conditional variables */
	int status = 0;
	pid_t pid, wpid;
	int num_child = 0;	/* number of children */
	int child_num = 0;	/* number of child */

	/* ask user to enter positive integer between 1 and 10 */
	while (1) {
		printf("how many child processes? (1 - 10)\n");
	
		int nr;
		char buf[16];

		nr = read(STDIN_FILENO, buf, sizeof(char)*16);
		if (nr < 1) {
			perror("something went wrong");
			exit(EXIT_FAILURE);
		}

		num_child = atoi(buf);

		if (num_child < 1 || num_child > 10) {
			printf("invalid number of children! try again\n");
		}
		else {
			break;
		}
	}

	/* spawn a number of children equal to the value entered */
	while (child_num < num_child) {
		child_num++;

		pid = fork();

		if (pid == -1) {
			perror("could not create child!");
			exit(EXIT_FAILURE);
		}
		if (pid == 0) {
			printf("I'm child number %i\n", child_num);
			exit(EXIT_SUCCESS);
		}
	}

	/* wait for every child to complete assigned task */
	while (wpid = wait(&status) > 0);

	/* print message "we are all done" */
	printf("We are all done!\n");

	/* exit */
}
