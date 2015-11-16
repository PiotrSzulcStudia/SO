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

#ifdef DEBUG
	static const bool debug = true;
#else
	static const bool debug = false;
#endif

void write_to_result(char* result, const char str);
void write_to_stack(char* stack, const char op, int *stack_pos);
char *str_dup (const char *s);

int main(int argc, char** argv)
{
	/*
	 * These counters will be used as offsets for messages when using pipes
	 * i.e. write(stack + stack_pos, strlen(stack + stack_pos))
	 */
	int stack_pos = 0;
	int input_pos = 1;

	char input[MAX_BUF_SIZE] = "";
	char stack[MAX_BUF_SIZE] = "";
	char result[MAX_BUF_SIZE] = "";

	fgets(input, MAX_BUF_SIZE - 1, stdin);
	fgets(stack, MAX_BUF_SIZE - 1, stdin);
	fgets(result, MAX_BUF_SIZE - 1, stdin);

	input[strcspn(input, "\n")] = 0;
	stack[strcspn(stack, "\n")] = 0;
	result[strcspn(result, "\n")] = 0;

	if ((strlen(input) == 0)) // End of input, clearing stack
	{
		while (stack_pos < strlen(stack))
		{
				write_to_result(result, ' ');
				write_to_result(result, stack[stack_pos++]);
		}

		if (debug)
			fprintf(stderr, "pid: %d end of input! result: %s \n",
				getpid(), result
			);

		printf("%s", result);
		return 0;
	}

	if (debug)
		fprintf(stderr, "pid: %d input[0]: %c stack: %s result: %s \n",
			getpid(), input[0], stack, result
		);

	bool add_to_stack = false;
	switch (input[0])
	{
		case '-':
			/* special case with negative numbers */
			if ((strlen(input) > 1) && (!isspace(input[1])))
			{
				write_to_result(result, input[0]);
				break;
			}
		case '+':
			while(stack_pos < strlen(stack))
			{
				add_to_stack = false;
				if(stack[stack_pos] != '(')
					add_to_stack = true;
		case '*':
		case '/':
		case '^':
				if ((stack[stack_pos] == '*') ||
					(stack[stack_pos] == '/') ||
					(stack[stack_pos] == '^') ||
					add_to_stack
				)
				{
					write_to_result(result, ' ');
					write_to_result(result, stack[stack_pos]);
					stack_pos++;
				}
					else break;
			}
			write_to_result(result, ' ');
		case '(':
			write_to_stack(stack, input[0], &stack_pos);
			break;
		case ')':
			while(stack_pos <= strlen(stack))
			{
				if (stack[stack_pos] != '(')
				{
					write_to_result(result, ' ');
					write_to_result(result, stack[stack_pos++]);
				}
				else
				{
					 stack_pos++;
					 break;
				}
			}
		case ' ':
		case '\n':
			break;
		default:
			/*
			 * We only checked first character of expression (input[0])
			 * Now let's check if there are other characters in expression
			 */
			for (int i = 0; i < strlen(input); i++)
			{
				/* When there's no space around parenthesis
				 * easier testing
				 */
				if (!isspace(input[i]) && (input[i] != ')'))
				{
					write_to_result(result, input[i]);
				}
				else
				{
					input_pos = i + ((input[i] != ')') ? (1) : (0));
					break;
				}
			}
			break;
	}

	/* Moving offset to first nonwhite character in input */
	while ((input_pos < strlen(input)) && (isspace(input[input_pos]))) input_pos++;

	spawn_worker(input + input_pos, stack + stack_pos, result);

	return 0;
}

char *str_dup (const char *s)
{
	char *d = malloc (strlen (s) + 1);
	if (d == NULL) return NULL;
	strcpy (d,s);
	return d;
}

void write_to_result(char* result, const char str)
{
	char add[2] = {str, '\0'};
	strcat(result, add);
	return;
}

void write_to_stack(char* stack, const char op, int *stack_pos)
{
	char operator[2] = {op, '\0'};
	char *tmp = str_dup(stack + *(stack_pos));
	strcpy(stack, operator);
	strcat(stack, tmp);
	free(tmp);
	*(stack_pos) = 0;
}

/* vim: set tabstop=8 softtabstop=8 shiftwidth=8 noexpandtab : */
