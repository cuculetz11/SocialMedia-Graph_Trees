#include "feed.h"
#include "posts.h"
#include "users.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int is_clique(int **friends, int *group, int size)
{
	(void)friends;
	(void)group;
	(void)size;
	for (int i = 0; i < size; i++) {
		for (int j = 0; j < size; j++) {
			if (i != j && !friends[group[i]][group[j]])
				return 0;
		}
	}
	return 1;
}

int number_nodes(int **friends)
{
	int count = 0;
	for (int i = 0; i < MAX_PEOPLE; i++) {
		for (int j = 0; j < MAX_PEOPLE; j++) {
			if (friends[i][j] == 1)
				count++;
		}
	}
	return count;
}

void max_clique(int **friends, int count, int node,
				int start, int size, int *group,
				int *max_size, int *max_group)
{
	if (size > *max_size) {
		*max_size = size;
		for (int i = 0; i < size; i++)
			max_group[i] = group[i];
	}

	if (size == count)
		return;

	for (int i = start; i < MAX_PEOPLE; i++) {
		if (i != node) {
			group[size] = i;
			if (is_clique(friends, group, size + 1))
				max_clique(friends, count, node, i + 1, size + 1, group,
						   max_size, max_group);
		}
	}
}

void sort_friends(int *group, int size)
{
	for (int i = 0; i < size - 1; i++) {
		int min = i;
		for (int j = i + 1; j < size; j++) {
			if (group[min] > group[j])
				min = j;
		}
		int aux = group[i];
		group[i] = group[min];
		group[min] = aux;
	}
}

void caut_repost_user(g_tree_t *events_tree, int user_id, char *titlu)
{
	queue_t *q = q_create(sizeof(g_node_t *), events_tree->size_tree);
	q_enqueue1(q, &events_tree->root);

	while (!q_is_empty(q)) {
		g_node_t **node_ptr = (g_node_t **)q_front(q);
		q_dequeue(q);
		g_node_t *node = *node_ptr;

		if (node->value->user_id == user_id && !node->value->events)
			printf("Reposted: %s\n", titlu);

		for (int i = 0; i < node->n_children; i++)
			q_enqueue1(q, &node->children[i]);
	}

	q_free(q);
}

void scot_useri_repost(g_tree_t *events_tree, int *vectori_useri)
{
	queue_t *q = q_create(sizeof(g_node_t *), events_tree->size_tree);
	q_enqueue1(q, &events_tree->root);
	int i = 0;
	while (!q_is_empty(q)) {
		g_node_t **node_ptr = (g_node_t **)q_front(q);
		q_dequeue(q);
		g_node_t *node = *node_ptr;

		if (!node->value->events) {
			vectori_useri[i] = node->value->user_id;
			//scot userii care au repostat aceea postare
			i++;
		}

		for (int i = 0; i < node->n_children; i++)
			q_enqueue1(q, &node->children[i]);
	}
	vectori_useri[i] = -1;
	q_free(q);
}

void feed(linked_list_t *posts_list, int id, int feed_size, int **friends)
{
	int nr_friends = 0;
	for (int i = 0; i < MAX_PEOPLE; i++) {
		if (friends[id][i] == 1)
			nr_friends++;
	}
	nr_friends++;
	//nr de prieteni inclusiv el
	int *vectori_prieteni = calloc(nr_friends, sizeof(int));
	//acest vector contine id-urile prietenilor
	int j = 0;
	for (int i = 0; i < MAX_PEOPLE; i++) {
		if (friends[id][i] == 1 || i == id) {
			vectori_prieteni[j] = i;
			j++;
		}
	}
	int *vector_index_postari = calloc(posts_list->size, sizeof(int));
	ll_node_t *aux = posts_list->head;
	int i = 0;
	while (aux) {
		node_post_t *post = (node_post_t *)aux->data;
		int user_post = post->user_id;
		//introduc numai postarile prietenilor
		for (int j = 0; j < nr_friends; j++) {
			if (vectori_prieteni[j] == user_post) {
				vector_index_postari[i] = post->id;
				i++;
			}
		}
		aux = aux->next;
	}
	int *vector_index_postari2 = calloc(i, sizeof(int));
	for (int k = 0; k < i; k++)
		vector_index_postari2[k] = vector_index_postari[k];

	free(vector_index_postari);
	vector_index_postari = vector_index_postari2;

	for (int k = 0; k < i - 1; k++) {
		for (int j = k; j < i; j++) {
			if (vector_index_postari[k] < vector_index_postari[j]) {
				int aux = vector_index_postari[k];
				vector_index_postari[k] = vector_index_postari[j];
				vector_index_postari[j] = aux;
			}
		}
	}
	//am redimensuinat si sortat vectorul de postari
	if (i < feed_size)
		feed_size = i;

	for (int k = 0; k < feed_size; k++) {
		node_post_t *post = return_post(posts_list,
										vector_index_postari[k]);
		printf("%s: %s\n", get_user_name(post->user_id), post->name);
	}
	free(vectori_prieteni);
	free(vector_index_postari);
}

void view_profile(linked_list_t *posts_list, int id)
{
	ll_node_t *aux = posts_list->head;
	//mai intai afisez postarile userului
	while (aux) {
		node_post_t *post = (node_post_t *)aux->data;
		if (post->user_id == id)
			printf("Posted: %s\n", post->name);

		aux = aux->next;
	}
	ll_node_t *aux2 = posts_list->head;
	while (aux2) {
		node_post_t *post = (node_post_t *)aux2->data;
		if (post->events)
			caut_repost_user(post->events, id, post->name);
		//afisez repostarile userului
		aux2 = aux2->next;
	}
}

void friend_repost(g_tree_t *events_tree, int id, int **friends)
{
	int *vectori_useri = calloc(events_tree->size_tree, sizeof(int));
	//in acest vector o sa am userii care au repostat postarea
	scot_useri_repost(events_tree, vectori_useri);

	for (int i = 0; vectori_useri[i] != -1; i++) {
		//vad care din useri sunt prieteni cu id
		if (friends[id][vectori_useri[i]] == 1)
			printf("%s\n", get_user_name(vectori_useri[i]));
	}
	free(vectori_useri);
}

void common_group(int **friends, int id, char *name)
{
	int count = number_nodes(friends);

	int *group = malloc(MAX_PEOPLE * sizeof(int));
	int *max_group = malloc(MAX_PEOPLE * sizeof(int));
	int max_size = 0;

	group[0] = id;
	max_clique(friends, count, id, 0, 1, group, &max_size, max_group);

	// trebuie sa sortez max_group
	sort_friends(max_group, max_size);

	printf("The closest friend group of %s is:\n", name);
	for (int i = 0; i < max_size; i++)
		printf("%s\n", get_user_name(max_group[i]));

	free(group);
	free(max_group);
}

void handle_input_feed(char *input, linked_list_t *posts_list,
					   int **friends)
{
	char *commands = strdup(input);
	char *cmd = strtok(commands, "\n ");
	friends = friends;
	if (!cmd)
		return;
	if (!strcmp(cmd, "feed")) {
		char *nume = strtok(NULL, "\n ");
		int id = get_user_id(nume);
		int feed_size = atoi(strtok(NULL, "\n "));
		feed(posts_list, id, feed_size, friends);

	} else if (!strcmp(cmd, "view-profile")) {
		char *nume = strtok(NULL, "\n ");
		int id = get_user_id(nume);
		view_profile(posts_list, id);
	} else if (!strcmp(cmd, "friends-repost")) {
		char *nume = strtok(NULL, "\n ");
		int id = get_user_id(nume);
		int id_post = atoi(strtok(NULL, "\n "));
		node_post_t *post = return_post(posts_list, id_post);
		friend_repost(post->events, id, friends);
	} else if (!strcmp(cmd, "common-group")) {
		char *name = strtok(NULL, "\n ");
		int id = get_user_id(name);
		common_group(friends, id, name);
	}

	free(commands);
}
