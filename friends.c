#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "friends.h"
#include "users.h"

void suggestion_helper(char *name, int ***friends)
{
	//helper for suggestion function
	uint16_t id = get_user_id(name);

	int count = 0;

	int *suggestions = malloc(MAX_PEOPLE * sizeof(int));
	for (int i = 0; i < MAX_PEOPLE; i++)
		suggestions[i] = 0;

	for (int i = 0; i < MAX_PEOPLE; i++) {
		//if the users i and id are friends, then check all of i's friends
		if ((*friends)[id][i]) {
			for (int j = 0; j < MAX_PEOPLE; j++) {
				//if the friends j is friend with i and not with id
				//it is added to the suggestions array
				if ((*friends)[i][j] && !(*friends)[id][j] && j != id) {
					count++;
					suggestions[j] = 1;
				}
			}
		}
	}
	if (!count) {
		printf("There are no suggestions for %s\n", name);
	} else {
		printf("Suggestions for %s:\n", name);

		for (int i = 0; i < MAX_PEOPLE; i++) {
			if (suggestions[i])
				printf("%s\n", get_user_name(i));
		}
	}
	free(suggestions);
}

void common_helper(char *name1, char *name2, int ***friends)
{
	uint16_t id1 = get_user_id(name1);
	uint16_t id2 = get_user_id(name2);

	int *common = malloc(MAX_PEOPLE * sizeof(int));
	for (int i = 0; i < MAX_PEOPLE; i++)
		common[i] = 0;
	int count = 0; //used for checking if they have common friends or not
	for (int i = 0; i < MAX_PEOPLE; i++) {
		if ((*friends)[id1][i] && (*friends)[id2][i]) {
			common[i] = 1;
			count++;
		}
	}

	if (!count) {
		printf("No common friends for %s and %s\n", name1, name2);
	} else {
		printf("The common friends between %s and %s are:\n", name1, name2);

		for (int i = 0; i < MAX_PEOPLE; i++) {
			if (common[i])
				printf("%s\n", get_user_name(i));
		}
	}
	free(common);
}

int friends_count(uint16_t id, int ***friends)
{
	int count = 0;
	for (int i = 0; i < MAX_PEOPLE; i++) {
		if ((*friends)[id][i])
			count++;
	}
	return count;
}

int distance_helper(uint16_t id1, uint16_t id2, int ***friends)
{
	int *visited = malloc(MAX_PEOPLE * sizeof(int));
	for (int i = 0; i < MAX_PEOPLE; i++)
		visited[i] = 0; //if the user is visited
	int *distances = malloc(MAX_PEOPLE * sizeof(int));
	for (int i = 0; i < MAX_PEOPLE; i++)
		distances[i] = -1; //distance between the id1 and user
	int *queue = malloc(MAX_PEOPLE * sizeof(int));
	for (int i = 0; i < MAX_PEOPLE; i++)
		queue[i] = 0;
	int front = 0, rear = 0;

	visited[id1] = 1;
	distances[id1] = 0;

	queue[rear++] = id1;
	while (front < rear) {
		int aux = queue[front++];
		for (int j = 0; j < MAX_PEOPLE; j++) {
			if ((*friends)[aux][j] && !visited[j]) {
				visited[j] = 1;
				distances[j] = distances[aux] + 1;
				queue[rear++] = j;

				if (j == id2) {
					free(visited);
					int result = distances[id2];
					free(distances);
					free(queue);
					return result;
				}
			}
		}
	}

	free(visited);
	free(distances);
	free(queue);
	return -1;
}

void popular_helper(char *name, int ***friends)
{
	uint16_t id = get_user_id(name);

	int fr = friends_count(id, friends);

	int id_max = id, max = fr;
	int aux;
	for (int i = 0; i < MAX_PEOPLE; i++) {
		if ((*friends)[id][i]) {
			aux = friends_count(i, friends);
			if (aux > max) {
				max = aux;
				id_max = i;
			}
		}
	}

	if (id == id_max)
		printf("%s is the most popular\n", name);
	else
		printf("%s is the most popular friend of %s\n",
			   get_user_name(id_max), name);
}

void handle_input_friends(char *input, int ***friends)
{
	char *commands = strdup(input);
	char *cmd = strtok(commands, "\n ");

	if (!cmd)
		return;

	if (!strcmp(cmd, "add")) {
		char *name1 = strtok(NULL, " ");
		char *name2 = strtok(NULL, "\n");

		uint16_t id1 = get_user_id(name1);
		uint16_t id2 = get_user_id(name2);

		(*friends)[id1][id2] = 1;
		(*friends)[id2][id1] = 1;

		printf("Added connection %s - %s\n", name1, name2);
	} else if (!strcmp(cmd, "remove")) {
		char *name1 = strtok(NULL, " ");
		char *name2 = strtok(NULL, "\n");

		if (!name1 || !name2)
			return;

		uint16_t id1 = get_user_id(name1);
		uint16_t id2 = get_user_id(name2);

		(*friends)[id1][id2] = 0;
		(*friends)[id2][id1] = 0;

		printf("Removed connection %s - %s\n", name1, name2);
	} else if (!strcmp(cmd, "suggestions")) {
		char *name = strtok(NULL, "\n");
		if (!name)
			return;

		suggestion_helper(name, friends);
	} else if (!strcmp(cmd, "distance")) {
		char *name1 = strtok(NULL, " ");
		char *name2 = strtok(NULL, "\n");

		if (!name1 || !name2)
			return;

		uint16_t id1 = get_user_id(name1);
		uint16_t id2 = get_user_id(name2);

		int distance = distance_helper(id1, id2, friends);
		if (distance == -1)
			printf("There is no way to get from %s to %s\n", name1, name2);
		else
			printf("The distance between %s - %s is %d\n",
				   name1, name2, distance);
	} else if (!strcmp(cmd, "common")) {
		char *name1 = strtok(NULL, " ");
		char *name2 = strtok(NULL, "\n");

		if (!name1 || !name2)
			return;

		common_helper(name1, name2, friends);
	} else if (!strcmp(cmd, "friends")) {
		char *name = strtok(NULL, "\n");
		if (!name)
			return;

		uint16_t id = get_user_id(name);

		int count = friends_count(id, friends);
		printf("%s has %d friends\n", name, count);
	} else if (!strcmp(cmd, "popular")) {
		char *name = strtok(NULL, "\n");
		popular_helper(name, friends);
	}
	free(commands);
}
