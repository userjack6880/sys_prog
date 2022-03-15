/*
 * Student Name:        John Bradley
 * Student NetID:       jeb446
 * Compiler Used:       gcc
 * Program Description:
 *	exercise 4 - this program handles multiple signals from the child
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <signal.h>
#include <time.h>
#include <sys/time.h>
#include <sys/wait.h>
#include <errno.h>

static int kill_child, print_val, kill_parent, lowValue, highValue;
static int alarm_val = 15;

void sigHand(int sig){
	if (sig == SIGINT) {
		int rd;
		int wr;
		char *exitQuestion;
		char exitAns[8];
		exitQuestion = "\nExit: Are you sure (Y/n)? ";
		while (1) {
			if (wr = write(STDOUT_FILENO, exitQuestion, strlen(exitQuestion)*sizeof(char)) > 0) {
				rd = read(STDIN_FILENO, exitAns, 8*sizeof(char));
				if ( strlen(exitAns) > 2 ) {
					if (exitAns[0] == 'y' || exitAns[0] == 'Y') {
						kill_parent = 1;
						return;
					}
					if (exitAns[0] == 'n' || exitAns[0] == 'N') return;
				}
			}
		}
	}
//	if (sig == SIGUSR1) {
//		lowValue = 1;
//		return;
//	}
//	if (sig == SIGUSR2) highValue = 1;
//	if (sig == SIGCHLD) {
//		int status = 0;
//		pid_t wpid;
//		while (wpid = wait(&status) > 0);
//	}
}

void sigHandChild(int sig) {
	if (sig == SIGINT) {
		perror("shouldn't have handled this...");
		exit(EXIT_FAILURE);
	}
	if (sig == SIGTERM) kill_child = 0;
	if (sig == SIGALRM) print_val = 1;
}

int main (int argc, char *argv[]) {
	/* setup the alarm */
	struct itimerval alrmval;
	alrmval.it_interval.tv_sec = 0;
	alrmval.it_interval.tv_usec = 0;
	alrmval.it_value.tv_sec = alarm_val;
	alrmval.it_value.tv_usec = 0;

	/* create the sigaction struct */
	struct sigaction sigAct;
	sigAct.sa_flags = 0;
	sigemptyset(&sigAct.sa_mask);

	/* fork! */
	pid_t pid = fork();

	/* check if failed */
	if (pid == -1) {
		perror("could not open child");
		exit(EXIT_FAILURE);
	}

	/* initialize signal handlers */
	/* child */
	if (pid == 0) {
		/* setup ignoring SIGINT */
		sigaddset(&sigAct.sa_mask, SIGINT);
		sigAct.sa_handler = SIG_IGN;
		if (sigaction(SIGINT, &sigAct, NULL) == -1) {
			perror("Could not ignore SIGINT");
			exit(EXIT_FAILURE);
		}

		/* have the child handle SIGTERM */
		sigAct.sa_handler = sigHandChild;
		sigaddset(&sigAct.sa_mask, SIGTERM);
		sigaddset(&sigAct.sa_mask, SIGALRM);
		if (sigaction(SIGTERM, &sigAct, NULL) == -1) {
			perror("Could not handle SIGTERM");
			exit(EXIT_FAILURE);
		}
		if (sigaction(SIGALRM, &sigAct, NULL) == -1) {
			perror("Could not handle SIGALRM");
			exit(EXIT_FAILURE);
		}

		setitimer(ITIMER_REAL, &alrmval, NULL);

		kill_child = 1;
		print_val = 0;

		/* initialize random */
		srand(time(0));

		/* do nothing loop, child does everything in the signal handler */
		while (kill_child) {
			if (print_val == 1) {
				int randVal = rand() % 101;
				printf("print a value: %i\n", randVal);
//				if (randVal < 25) raise(SIGUSR1);
//				else if (randVal > 75) raise(SIGUSR2);
//				else exit(EXIT_SUCCESS);
				print_val = 0;
				alrmval.it_value.tv_sec = alarm_val;
				setitimer(ITIMER_REAL, &alrmval, NULL);
			}
		}

		/* this should be after SIGTERM was issued */
		exit(EXIT_SUCCESS);
	}
	/* parent */
	else {
		lowValue = 0;
		highValue = 0;
		kill_parent = 0;

		/* create the sigaction struct */
		struct sigaction sigAct;
		sigAct.sa_flags = 0;
		sigemptyset(&sigAct.sa_mask);

		sigAct.sa_handler = sigHand;

		sigaddset(&sigAct.sa_mask, SIGCHLD);
		sigaddset(&sigAct.sa_mask, SIGINT);
		sigaddset(&sigAct.sa_mask, SIGUSR1);
		sigaddset(&sigAct.sa_mask, SIGUSR2);

		if (sigaction(SIGINT, &sigAct, NULL) == -1) {
			perror("Could not handle SIGINT");
			exit(EXIT_FAILURE);
		}
		if (sigaction(SIGUSR1, &sigAct, NULL) == -1) {
			perror("Could not handle SIGUSR1");
			exit(EXIT_FAILURE);
		}
		if (sigaction(SIGUSR2, &sigAct, NULL) == -1) {
			perror("Could not handle SIGUSR2");
			exit(EXIT_FAILURE);
		}
		if (sigaction(SIGCHLD, &sigAct, NULL) == -1) {
			perror("Could not handle SIGCHLD");
			exit(EXIT_FAILURE);
		}

		/* main loop... not much here */
		while(1) {
			if (lowValue == 1) printf("lowval");
			if (highValue == 1) printf("highval");
			/* if the kill flag is set... */
			if (kill_parent) {
				if (kill(pid, SIGTERM) == -1) {
					perror("could not kill child!");
					exit(EXIT_FAILURE);
				}
				else {
					printf("killed child!\n");
					break;
				}
			}
		}
	}
}
