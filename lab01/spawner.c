/* Piotr Szulc ps347277 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>

#include "err.h"
#include "spawner.h"

void spawn_worker(char* input, char* stack, char* result)
{
	int pipes[NUM_PIPES][2];
	char buffer[MAX_BUF_SIZE] = "";

	if ((pipe(PARENT_READ_PIPE) == -1) ||
		(pipe(PARENT_WRITE_PIPE) == -1)
	)
		syserr("pipe\n");

	switch (fork())
	{
		case -1:
			syserr("fork\n");

		case 0:
			if ((dup2(CHILD_READ_FD, STDIN_FILENO) == -1) ||
				(dup2(CHILD_WRITE_FD, STDOUT_FILENO) == -1)
			)
				syserr("dup2\n");

			for (int i = 0; i < NUM_PIPES; i++)
			{
				for (int j = 0; j < 2; j++)
				{
					if (close(pipes[i][j]) == -1)
						syserr("close\n");
				}
			}

			execl("./W", "W", NULL);
			syserr("exec\n");

		default:
			if ((close(CHILD_READ_FD) == -1) ||
				(close(CHILD_WRITE_FD) == -1)
			)
				syserr("close\n");

			write_to_child_with_check(PARENT_WRITE_FD, input);
			write_to_child_with_check(PARENT_WRITE_FD, stack);
			write_to_child_with_check(PARENT_WRITE_FD, result);

			int buf_len = 0;
			if ((buf_len = read(PARENT_READ_FD, buffer, MAX_BUF_SIZE - 1)) == -1)
				syserr("read\n");
			printf("%s", buffer);

			if (wait(0) == -1)
				syserr("wait\n");

			return;
	}
}

void write_to_child_with_check(int fd, char *msg)
{
	if (write(fd, msg, strlen(msg)) == -1)
		syserr("write\n");
	if (write(fd, "\n", strlen("\n")) == -1)
		syserr("write\n");
}

/* vim: set tabstop=8 softtabstop=8 shiftwidth=8 noexpandtab : */
