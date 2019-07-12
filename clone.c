#define _GNU_SOURCE
#include <sched.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>

#define STACKSIZE (1024 * 1024)

struct child_args {
	char **argv;
};

static int childfunc(void *arg) {

	struct child_args *args = arg;
	
	if (execvp(args->argv[0], args->argv) == -1) {
		printf("failure to launch %s", (char *)arg);
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}


int main(int argc, char **argv) {

	struct child_args args;
	char *childstack;
	int childflags;
	pid_t pid;

	if (argc < 2) {
		printf("Not enough args\n"); 
	}

	args.argv = &argv[1];

	childflags = SIGCHLD;

	childstack = malloc(STACKSIZE);
	if (childstack == NULL) {
		printf("error allocating stack\n");
		return EXIT_FAILURE;
	}

	pid = clone(childfunc, childstack + STACKSIZE, childflags, &args);

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