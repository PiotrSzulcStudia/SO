/* Piotr Szulc ps347277 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include <limits.h>

#include "err.h"

#define MAX_BUF_SIZE 32768
#define NUM_PIPES 2

#define PARENT_READ_PIPE                   pipes[0]
#define PARENT_WRITE_PIPE                  pipes[1]
#define PARENT_READ_FD          PARENT_READ_PIPE[0]
#define PARENT_WRITE_FD        PARENT_WRITE_PIPE[1]
#define CHILD_READ_FD          PARENT_WRITE_PIPE[0]
#define CHILD_WRITE_FD          PARENT_READ_PIPE[1]

/*
 * Forks and executes program W in present directory;
 * Father sends three strings separated by '\n': @input, @stack, @result
 * and prints output result from child
 */
void spawn_worker(char* input, char* stack, char* result);

/*
 * Adds '\n' after each message
 */
void write_to_child_with_check(int fd, char *msg);

/* vim: set tabstop=8 softtabstop=8 shiftwidth=8 noexpandtab : */
