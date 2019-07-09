#define _GNU_SOURCE
#include <sched.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>

#define STACKSIZE (1024 * 1024)


static int childfunc(void *arg) {

	char* argv[] = { "", NULL };

	if (execv((char *)arg, argv) == -1) {
		printf("failure to launch %s", (char *)arg);
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}


int main(int argc, char *argv[]) {

	char *childstack;
	int childflags;
	pid_t pid;


	if (argc < 2) {
		printf("Not enough args\n"); 
	}

	childflags = CLONE_NEWUTS | SIGCHLD;

	childstack = malloc(STACKSIZE);
	if (childstack == NULL) {
		printf("error allocating stack\n");
		return EXIT_FAILURE;
	}

	pid = clone(childfunc, childstack + STACKSIZE, childflags, argv[1]);

	if (pid < 0) {
		printf("failure to run child\n");
		return EXIT_FAILURE;
	}

	if (waitpid(pid, NULL, 0) == -1) {
		printf("failed to close properly\n");
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}