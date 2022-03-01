/*
 * Student Name:        John Bradley
 * Student NetID:       jeb446
 * Compiler Used:       gcc
 * Program Description:
 *	test 1 program
 */

/* All of the includes for the project go here ...
 * those includes must support signals, interval timers,
 * read, open, and close. You will also need support for
 * error numbers
 */
#include <stdio.h>
#include <stdlib.h>

/* Problem 1 - complete the list of include files */
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <signal.h>
#include <sys/time.h>
#include <errno.h>

#define BUF_SIZE 1024

static int timed;

/* the myPrint function takes a constant c-string
 * and outputs it to STDOUT using write
 * feel free to ue string functions like strcmp, strcat, etc
 * to make this one work
 * myPrint returns 0 on failure and 1 on success
 */
int myPrint(const char *str) {
	/* Problem 2 - implement myPrint using write */
	int bWr;
	if (bWr = write(STDOUT_FILENO, str, strlen(str)*sizeof(char)) == 0) return 1;
	return 0;
}

/* the myPrintInt function takes a constant int
 * and outputs it to STDOUT using write
 * feel free to use of a function like spritnf to convert
 * an integer to a string
 * myPrintInt returns 0 on failure and 1 on success
 * Note: you will need to convert the int to a c-string
 * to make it writeable
 */
int myPrintInt(const int val) {
	/* Problem 3 - Implement myPrintInt, you must use write or another function implemented in this program */
	int bWr;
	char str[8];
	sprintf(str, "%i", val);
	if (bWr = write(STDOUT_FILENO, str, strlen(str)*sizeof(char)) == 0) return 1;
	return 0;
}

/* implement the signal handler below ...
 * use the provited prototype
 * It must handle two signals, SIGINT and SIGALRM
 * if the signal handler receives SIGINT
 * prompt the user if they want to exit or not
 * on yes, exit successfully, on no, just return from the handler
 * on SIGALRM, set the timeout flag to 1
 */

void signalHandler(int sig) {
	/*	 Problem 4 - implement the signalHandler ...
	 * when writing the prompt for SIGINT and reading the reply
	 * use system calls ... please also remember to read more than 1
	 * character when reading the response since that response has at least
	 * two characters in it. Yes the responses of 'Y' and 'n' involve multiple characters
	 * Remember, that functions like sprintf and scanf aren't consider interrupt handler
	 * safe, use system calls or functions written in this program in their place.
	 */
	 
	if (sig == SIGINT) {
		int rd;
		int wr;
		char *exitQuestion; 
		char exitAns[8];
		exitQuestion = "\nDo you want to quit?\n";
		while (1) {
			if (wr = write(STDOUT_FILENO, exitQuestion, strlen(exitQuestion)*sizeof(char)) > 0) {
				rd = read(STDIN_FILENO, exitAns, 8*sizeof(char));
				if ( strlen(exitAns) > 2 ) {
					if (exitAns[0] == 'y' || exitAns[0] == 'Y') exit(EXIT_SUCCESS);
					if (exitAns[0] == 'n' || exitAns[0] == 'N') return;
				}
			}
		}
	}
	if (sig == SIGALRM) timed = 1;
}


/* This function reads a line of text from a file
 * we are simulating another function behavior here ...
 * the basic idea is to read the file character by character til we find
 * the \n or \r
 * Go ahead and tack the null character on here for simplicity
 * Return a 0 if no characters were read, return a 1 otherwise
 */
int readLine(int fd, char *line) {
	/* Problem 5 - implement readLine as described above ... again
	 * remember to use read to do this ... This is probably best accomplished by
	 * reading in 1 character at a time and then adding them to the array
	 * you passed in as line
	 */
	char buf[1];
	int rd;
	int pos = 0;
	while (rd = read(fd,buf,sizeof(buf)) > 0) {
		if (buf[0] == '\n' || buf[0] == '\r') {
			line[pos] = 0;
			return 1;
		}
		else {
			line[pos] = buf[0];
		}
		pos++;
	}
	return 0;
}

/* This function reads a question answer pairing
 * from the previous pair of file descriptors
 * it returns 0 if the files are empty
 * and 1 if it successfully reads the pairing
 * notice it calls the custom function readLine since we
 * need to read only one line at a time
 */
int readQA(int questFd, int ansFd, char *quest, char*ans) {
	if (readLine(questFd, quest) == 0) return 0;
	if (readLine(ansFd, ans) == 0) return 0;
	return 1;
}

int main (int argc, char *argv[]) {
	int numRead = 0;
	int numWrite = 0;
	int question = 1;
	int correct = 0;
	char buf[BUF_SIZE];
	char quest[BUF_SIZE];
	char ans[BUF_SIZE];
	int questFd, ansFd;

	/* declare structures for signals and timers.
	 * recall that you can only set one REAL timeer at a time,
	 * but that timer will need to set to two different delays ... the active delay of 30 seconds and
	 * the disable delay of 0 seconds. Treat each delay as a different instance of the timer
	 * structure. For simplicity, name the active delay structure tvOn and the disabled delay
	 * structure tvOff.
	 */

	/* Problem 6a - declare the structures for the signals and timers */
	struct sigaction sigAct;
	struct itimerval tvOn;
	struct itimerval tvOff;

	/* initialize structures for signals and timers
	 * make certain to actually set a mask this time
	 * add both SIGINT and SIGALRM to the mask
	 */

	/* Problem 6b - initialize the structure for the signal handlers */
	sigAct.sa_flags = 0;
	sigAct.sa_handler = signalHandler;

	sigemptyset(&sigAct.sa_mask);
	sigaddset(&sigAct.sa_mask, SIGINT);
	sigaddset(&sigAct.sa_mask, SIGALRM);

	/* The initialization for the timer structurers follow.
	 * First, set initialize the timer's values for the active delay (tvOn)
	 * you get 30 seconds a question ...
	 * this timer has no interval, just a value
	 */

	/* Problem 6c - initialize the structure for the 30 sec timer */
	tvOn.it_interval.tv_sec = 0;
	tvOn.it_interval.tv_usec = 0;
	tvOn.it_value.tv_sec = 30;
	tvOn.it_value.tv_usec = 0;

	/* Set the timer's value for the disabled delay (tvOff)
	 * you can disable the timer when testing
	 * by setting interval and value to 0
	 */

	/* Problem 6d - initialize the structure for the 0 sec timer */
	tvOff.it_interval.tv_sec = 0;
	tvOff.it_interval.tv_usec = 0;
	tvOff.it_value.tv_sec = 0;
	tvOff.it_value.tv_usec = 0;

	/* set up the signal handlers */
	/* Problem 7 - associate the SIGINT and SIGALRM with their signal handler */
	sigaction(SIGINT, &sigAct, NULL);
	sigaction(SIGALRM, &sigAct, NULL);

	/* open the question and answer files ... these are simple text files
	 * called quest.txt and ans.txt
	 */

	/* Problem 8 - open the files quest.txt and ans.txt */
	questFd = open("quest.txt",O_RDONLY);
	ansFd = open("ans.txt",O_RDONLY);

	/* this loop handles the Q/A stuff
	 * I have included some of it to make your life simpler
	 * I have also left some comments to assist you as well
	 *
	 * read the first question, answer pairng prior to entering the loop
	 */

	readQA(questFd, ansFd, quest, ans);
	while (1) {
		/* output the current question */
		myPrint("#");
		myPrintInt(question);
		myPrint(" ");
		myPrint(quest);
		myPrint("? ");
		/* we will set the timedout flag to 0, since it hasn't yet */
		timed = 0;

		/* now set the interval timer prior to reading in the user's response */
		/* Problem 9 - set the interval time to it's active delay structure */
		setitimer(ITIMER_REAL, &tvOn, NULL);

		/* read in the user's response from STDIN, this is systems programming
		 * so I am forbidding you from using scanf */
		/* Problem 10 - read a buffer from STDIN */

		numRead = read(STDIN_FILENO, buf, sizeof(buf));

		/* test to see if the user responded and if an error has occured
		 * an error can actually occur as part of the functionality of this program
		 * both SIGINT and SGALRM will cause the read call to return a -1
		 * this can be recognized by testing for a specific error number ...
		 * The error is known as interrupted systems call
		 * Should this be the case, do the following ...
		 * If the read call was interrupted and the timedout flag isn't set
		 * just continue
		 * if the timedout flag was set, inform the user that time is up
		 * then go to the next question
		 */

		if (numRead == 0) break;
		if (numRead == -1) {
			if (errno == EINTR) {
				if (timed) {
					myPrint("\nTime's up, next question\n");
					if (readQA(questFd, ansFd, quest, ans) == 0) break;
					question++;
				}
				continue;
				perror("read");
				exit(EXIT_FAILURE);
			}
		}
		/* disable the timer here ...
		 * we are doing stuff that takes time
		 * you can disable the timer by passing the disable delay structure
		 * to the timer
		 */
		/* Problem 11 - disable the time by settng it to tvOff */
		setitimer(ITIMER_REAL, &tvOff, NULL);

		/* we will convert the buf being read in to a c-string by tacking on a 0 */
		buf[numRead-1] = 0;
		/* check the answer */
		if (strcmp(buf,ans) == 0) {
			correct++;
			myPrint("\ncorrect\n");
		}
		else {
			myPrint(ans);
			myPrint("\nwrong\n");
		}
		/* read the next question ... break if there are no more questions */
		if (readQA(questFd, ansFd, quest, ans) == 0) break;

		question++;
	}

	myPrint("final score is ");
	myPrintInt(correct);
	myPrint(" out of ");
	myPrintInt(question);
	myPrint("\n");

	/* Problem 12 - close both files */
	close(questFd);
	close(ansFd);
}
