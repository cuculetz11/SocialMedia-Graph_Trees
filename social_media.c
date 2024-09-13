/**
 * The entrypoint of the homework. Every initialization must be done here
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "feed.h"
#include "friends.h"
#include "posts.h"
#include "users.h"

/**
 * Initializez every task based on which task we are running
 */

void init_tasks(void)
{
#ifdef TASK_1

#endif

#ifdef TASK_2

#endif

#ifdef TASK_3

#endif
}

/**
 * Entrypoint of the program, compiled with different defines for each task
 */
int main(void)
{
	init_users();

	init_tasks();

// TASK 1
#ifdef TASK_1
	int **friends = malloc(MAX_PEOPLE * sizeof(int *));
	for (int i = 0; i < MAX_PEOPLE; i++) {
		friends[i] = malloc(MAX_PEOPLE * sizeof(int));
		for (int j = 0; j < MAX_PEOPLE; j++)
			friends[i][j] = 0;
	}
#endif
	// TASK 1

	// TASK 2
#ifdef TASK_2
	linked_list_t *posts_list = calloc(1, sizeof(linked_list_t));
	DIE(!posts_list, "calloc failed");
	posts_list->size = 0;
	posts_list->head = NULL;
#endif

	// TASK 2

	char *input = (char *)malloc(MAX_COMMAND_LEN);
	while (1) {
		char *command = fgets(input, MAX_COMMAND_LEN, stdin);

		// If fgets returns null, we reached EOF
		if (!command)
			break;

#ifdef TASK_1
		handle_input_friends(input, &friends);
#endif

#ifdef TASK_2
		handle_input_posts(input, posts_list);
#endif

#ifdef TASK_3
		handle_input_feed(input, posts_list, friends);
#endif
	}
#ifdef TASK_1

	for (int i = 0; i < MAX_PEOPLE; i++)
		free(friends[i]);
	free(friends);
#endif

#ifdef TASK_2
	free_post_list(posts_list);
	free(posts_list);
#endif

	free_users();
	free(input);

	return 0;
}
