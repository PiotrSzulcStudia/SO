/* Piotr Szulc ps347277 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>
#include <stdbool.h>
#include <limits.h>

#include "spawner.h"

#ifdef DEBUG
	static const bool debug = true;
#else
	static const bool debug = false;
#endif

int main(int argc, char** argv)
{
	char buffer[MAX_BUF_SIZE] = "";

	if ((argc < 2) || (!strcmp(argv[1],"")))
	{
		printf("Empty argument, exiting...\n");
		return 0;
	}

	if (debug) printf("ToONP PID = %d\n", getpid());

	strcpy(buffer, argv[1]);

	spawn_worker(buffer, "", "");

	return 0;
}

/* vim: set tabstop=8 softtabstop=8 shiftwidth=8 noexpandtab : */
