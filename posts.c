#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "posts.h"
#include "users.h"

linked_list_t *
ll_create(unsigned int data_size)
{
	linked_list_t *ll;

	ll = malloc(sizeof(*ll));

	ll->head = NULL;
	ll->data_size = data_size;
	ll->size = 0;

	return ll;
}

void ll_add_nth_node(linked_list_t *list, int n, const void *new_data)
{
	ll_node_t *prev, *curr;
	ll_node_t *new_node;

	if (!list)
		return;

	/* n >= list->size inseamna adaugarea unui nou nod la finalul listei. */
	if (n > list->size)
		n = list->size;

	curr = list->head;
	prev = NULL;
	while (n > 0) {
		prev = curr;
		curr = curr->next;
		--n;
	}

	new_node = malloc(sizeof(*new_node));
	// new_node->data = malloc(list->data_size);
	// memcpy(new_node->data, new_data, list->data_size);
	new_node->data = (void *)new_data;
	new_node->next = curr;
	if (!prev) {
		/* Adica n == 0. */
		list->head = new_node;
	} else {
		prev->next = new_node;
	}

	list->size++;
}

/*
 * Elimina nodul de pe pozitia n din lista al carei pointer este trimis ca
 * parametru. Pozitiile din lista se indexeaza de la 0 (i.e. primul nod din
 * lista se afla pe pozitia n=0). Daca n >= nr_noduri - 1, se elimina nodul de
 * la finalul listei. Daca n < 0, eroare. Functia intoarce un pointer spre acest
 * nod proaspat eliminat din lista. Este responsabilitatea apelantului sa
 * elibereze memoria acestui nod.
 */
ll_node_t *
ll_remove_nth_node(linked_list_t *list, int n)
{
	ll_node_t *prev, *curr;

	if (!list || !list->head)
		return NULL;

	/* n >= list->size - 1 inseamna eliminarea nodului de la finalul listei. */
	if (n > list->size - 1)
		n = list->size - 1;

	curr = list->head;
	prev = NULL;
	while (n > 0) {
		prev = curr;
		curr = curr->next;
		--n;
	}

	if (!prev) {
		/* Adica n == 0. */
		list->head = curr->next;
	} else {
		prev->next = curr->next;
	}

	list->size--;

	return curr;
}

/*
 * Functia intoarce numarul de noduri din lista al carei pointer este trimis ca
 * parametru.
 */
unsigned int
ll_get_size(linked_list_t *list)
{
	if (!list)
		return -1;

	return list->size;
}

/*
 * Procedura elibereaza memoria folosita de toate nodurile din lista, iar la
 * sfarsit, elibereaza memoria folosita de structura lista si actualizeaza la
 * NULL valoarea pointerului la care pointeaza argumentul (argumentul este un
 * pointer la un pointer).
 */
void ll_free(linked_list_t **pp_list)
{
	ll_node_t *currnode;

	if (!pp_list || !*pp_list)
		return;

	while (ll_get_size(*pp_list) > 0) {
		currnode = ll_remove_nth_node(*pp_list, 0);
		currnode->data = NULL;
		free(currnode);
		currnode = NULL;
	}

	//*pp_list = NULL;
}

/*
 * Atentie! Aceasta functie poate fi apelata doar pe liste ale caror noduri STIM
 * ca stocheaza int-uri. Functia afiseaza toate valorile int stocate in nodurile
 * din lista inlantuita separate printr-un spatiu.
 */
void ll_print_int(linked_list_t *list)
{
	ll_node_t *curr;

	if (!list)
		return;

	curr = list->head;
	while (curr) {
		printf("%d ", *((int *)curr->data));
		curr = curr->next;
	}

	printf("\n");
}

/*
 * Atentie! Aceasta functie poate fi apelata doar pe liste ale caror noduri STIM
 * ca stocheaza string-uri. Functia afiseaza toate string-urile stocate in
 * nodurile din lista inlantuita, separate printr-un spatiu.
 */
void ll_print_string(linked_list_t *list)
{
	ll_node_t *curr;

	if (!list)
		return;

	curr = list->head;
	while (curr) {
		printf("%s ", (char *)curr->data);
		curr = curr->next;
	}

	printf("\n");
}

stack *
st_create(unsigned int data_size)
{
	stack *st = malloc(sizeof(*st));
	st->list = ll_create(data_size);

	return st;
}

unsigned int
st_get_size(stack *st)
{
	if (!st || !st->list)
		return 0;
	return st->list->size;
}

/*
 * Intoarce 1 daca stiva este goala si 0 in caz contrar.
 */
unsigned int
st_is_empty(stack *st)
{
	return !st || !st->list || !st->list->size;
}

void *
st_peek(stack *st)
{
	if (!st || !st->list || !st->list->size)
		return NULL;

	return st->list->head->data;
}

void st_pop(stack *st)
{
	if (!st || !st->list)
		return;

	ll_node_t *old = ll_remove_nth_node(st->list, 0);
	// free(old->data);
	free(old);
}

void st_push(stack *st, void *new_data)
{
	if (!st || !st->list)
		return;

	ll_add_nth_node(st->list, 0, new_data);
}

void st_clear(stack *st)
{
	if (!st || !st->list)
		return;

	ll_free(&st->list);
}

void st_free(stack *st)
{
	if (!st || !st->list)
		return;

	ll_free(&st->list);
	free(st->list);
	free(st);
}

queue_t *
q_create(unsigned int data_size, unsigned int max_size)
{
	queue_t *q = calloc(1, sizeof(*q));

	q->data_size = data_size;
	q->max_size = max_size;

	q->buff = malloc(max_size * sizeof(*q->buff));

	return q;
}

/*
 * Functia intoarce numarul de elemente din coada al carei pointer este trimis
 * ca parametru.
 */
unsigned int
q_get_size(queue_t *q)
{
	return !q ? 0 : q->size;
}

/*
 * Intoarce 1 daca coada este goala si 0 in caz contrar.
 */
unsigned int
q_is_empty(queue_t *q)
{
	return !q ? 1 : !q->size;
}

/*
 * Functia intoarce primul element din coada, fara sa il elimine.
 */
void *
q_front(queue_t *q)
{
	if (!q || !q->size)
		return NULL;
	return q->buff[q->read_idx];
}

/*
 * Functia scoate un element din coada. Se va intoarce 1 daca operatia s-a
 * efectuat cu succes (exista cel putin un element pentru a fi eliminat) si
 * 0 in caz contrar.
 */
int q_dequeue(queue_t *q)
{
	if (!q || q->size == 0)
		return 0;

	q->read_idx = (q->read_idx + 1) % q->max_size;
	--q->size;
	return 1;
}

/*
 * Functia introduce un nou element in coada. Se va intoarce 1 daca
 * operatia s-a efectuat cu succes (nu s-a atins dimensiunea maxima)
 * si 0 in caz contrar.
 */
int q_enqueue(queue_t *q, void *new_data)
{
	void *data;
	if (!q || q->size == q->max_size)
		return 0;

	data = calloc(1, q->data_size);
	if (!data) {
		perror("Failed to allocate memory for queue data");
		return 0;
	}
	memcpy(data, new_data, q->data_size);

	q->buff[q->write_idx] = data;
	q->write_idx = (q->write_idx + 1) % q->max_size;
	++q->size;

	return 1;
}

/*
 * Functia elimina toate elementele din coada primita ca parametru.
 */
void q_clear(queue_t *q)
{
	unsigned int i;
	if (!q || !q->size)
		return;

	for (i = q->read_idx; i != q->write_idx; i = (i + 1) % q->max_size)
		free(q->buff[i]);

	q->read_idx = 0;
	q->write_idx = 0;
	q->size = 0;
}

/*
 * Functia elibereaza toata memoria ocupata de coada.
 */
void q_free(queue_t *q)
{
	if (!q)
		return;

	q_clear(q);
	free(q->buff);
	free(q);
}

void q_free_parcurgere(queue_t *q)
{
	if (!q)
		return;

	free(q->buff);
	free(q);
}

int q_enqueue1(queue_t *q, void *new_data)
{
	q->buff[q->write_idx] = new_data;
	q->write_idx = (q->write_idx + 1) % q->max_size;
	++q->size;

	return 1;
}

node_post_t *init_post(int id, char *name, int user_id)
{
	node_post_t *new_post = calloc(1, sizeof(node_post_t));
	DIE(!new_post, "calloc failed");
	new_post->id = id;
	new_post->name = strdup(name);
	new_post->user_id = user_id;
	new_post->post_likes = calloc(1, sizeof(like_t));
	new_post->post_likes->size = 0;
	new_post->post_likes->v = calloc(MAX_PEOPLE, sizeof(char));
	//facem un vector de like-uri pentru fiecare postare si repostare
	//fiecare element din vector va fi 0 sau 1
	new_post->events = calloc(1, sizeof(g_tree_t));
	//in events vom pune tot arborele specific unei postari
	new_post->events->root = calloc(1, sizeof(g_node_t));
	new_post->events->root->n_children = 0;
	new_post->events->root->children = calloc(1, sizeof(g_node_t *));
	new_post->events->root->parent = NULL;
	new_post->events->size_tree = 1;
	new_post->events->root->value = new_post;
	// stochez ca valoarea din tree fiecare postar,repostare,
	//rootul fiind o postare

	return new_post;
}

node_post_t *init_repost(int id, int user_id)
{
	node_post_t *new_post = calloc(1, sizeof(node_post_t));
	DIE(!new_post, "calloc failed");
	new_post->id = id;
	new_post->user_id = user_id;
	new_post->name = NULL;
	new_post->post_likes = calloc(1, sizeof(like_t));
	new_post->post_likes->size = 0;
	new_post->post_likes->v = calloc(MAX_PEOPLE, sizeof(char));
	new_post->events = NULL;
	//fac events-ul NULL pentru repostari
	return new_post;
}

node_post_t *return_post(linked_list_t *posts_list, int id)
{
	ll_node_t *current = posts_list->head;
	while (current) {
		node_post_t *post = (node_post_t *)current->data;
		if (post->id == id)
			return post;
		current = current->next;
	}
	return NULL;
}

g_node_t *parcurgere(g_tree_t *events_tree, int repost_id)
{
	//fac "bfs" pe arbore si scot repostarea cu id-ul dat
	queue_t *q = q_create(sizeof(g_node_t *), events_tree->size_tree);
	q_enqueue1(q, &events_tree->root);

	while (!q_is_empty(q)) {
		g_node_t **node_ptr = (g_node_t **)q_front(q);
		q_dequeue(q);
		g_node_t *node = *node_ptr;

		if (node->value->id == repost_id) {
			q_free_parcurgere(q);
			return node;
		}

		for (int i = 0; i < node->n_children; i++)
			q_enqueue1(q, &node->children[i]);
	}

	q_free(q);
	return NULL;
}

void free_post_list(linked_list_t *posts_list)
{
	ll_node_t *current = posts_list->head;
	while (current) {
		node_post_t *post = (node_post_t *)current->data;
		free_post_node(&post);
		current = current->next;
	}
	ll_free(&posts_list);
}

void free_post_node(node_post_t **post)
{
	if (!post)
		return;
	free((*post)->name);
	free((*post)->post_likes->v);
	free((*post)->post_likes);
	if ((*post)->events) {
		g_node_t ***nodes = free_g_tree((*post)->events);
		for (int i = 0; i < (*post)->events->size_tree; i++)
			free(nodes[i]);

		free(nodes);
		free((*post)->events);
	}

	free(*post);
}

g_node_t ***free_g_tree(g_tree_t *tree)
{
	g_node_t ***nodes = calloc(tree->size_tree, sizeof(g_node_t **));
	queue_t *q = q_create(sizeof(g_node_t *), tree->size_tree);
	int i = 0;
	q_enqueue1(q, &tree->root);
	while (!q_is_empty(q)) {
		g_node_t **node_ptr = (g_node_t **)q_front(q);
		q_dequeue(q);

		g_node_t *node = *node_ptr;
		if (!node->value->events)
			free_post_node(&node->value);
		for (int i = 0; i < node->n_children; i++)
			q_enqueue1(q, &node->children[i]);
		nodes[i] = node->children;
		i++;

		free(node);
	}

	q_free(q);
	return nodes;
}

g_node_t ***free_coarbore(g_node_t *root, int size)
{
	//retin ointerii la vectorul de copii in acest vector
	g_node_t ***nodes = calloc(size, sizeof(g_node_t **));
	int i = 0;
	queue_t *q = q_create(sizeof(g_node_t *), size);
	q_enqueue1(q, &root);
	while (!q_is_empty(q)) {
		g_node_t **node_ptr = (g_node_t **)q_front(q);
		q_dequeue(q);
		g_node_t *node = *node_ptr;
		for (int i = 0; i < node->n_children; i++)
			q_enqueue1(q, &node->children[i]);
		free_post_node(&node->value);
		nodes[i] = node->children;
		i++;
		free(node);
	}
	q_free(q);
	return nodes;
}

void rsd_parcurgere(g_node_t *node)
{
	stack *st = st_create(sizeof(g_node_t));
	st_push(st, node);
	while (!st_is_empty(st)) {
		g_node_t *aux = st_peek(st);
		st_pop(st);
		//verific daca e root(postare) sau repostare
		if (aux->value->events)
			printf("%s - Post by %s\n", aux->value->name,
				   get_user_name(aux->value->user_id));
		else
			printf("Repost #%d by %s\n", aux->value->id,
				   get_user_name(aux->value->user_id));

		for (int i = aux->n_children - 1; i >= 0; i--)
			st_push(st, aux->children[i]);
	}
	st_free(st);
}

void rsd_parcurgere_pt_lca(g_node_t *node, int id_repost)
{
	stack *st = st_create(sizeof(g_node_t));
	st_push(st, node);
	while (!st_is_empty(st)) {
		g_node_t *aux = st_peek(st);
		st_pop(st);
		printf("%d\n", aux->value->id);
		if (aux->value->id == id_repost) {
			st_free(st);
			return;
		}
		for (int i = aux->n_children - 1; i >= 0; i--) {
			st_push(st, aux->children[i]);
			printf("%d\n", aux->children[i]->value->id);
		}
	}
	st_free(st);
}

void caut_like_mare(g_node_t *node, int *index_repost,
					int like_post, int size_tree)
{
	//parcurg arborele cautand o repostare cu mai multe like-uri decat postarea
	queue_t *q = q_create(sizeof(g_node_t), size_tree);
	q_enqueue(q, node);

	while (!q_is_empty(q)) {
		g_node_t *node = q_front(q);
		q_dequeue(q);

		if (node->value->post_likes->size > like_post) {
			*index_repost = node->value->id;
			like_post = node->value->post_likes->size;
		} else if (node->value->post_likes->size == like_post &&
				   node->value->id < *index_repost) {
			*index_repost = node->value->id;
		}
		for (int i = 0; i < node->n_children; i++)
			q_enqueue(q, node->children[i]);
		free(node);
	}
	q_free(q);
}

int return_index_lista(linked_list_t *posts_list, int id)
{
	ll_node_t *current = posts_list->head;
	int index = 0;
	while (current) {
		node_post_t *post = (node_post_t *)current->data;
		if (post->id == id)
			return index;
		index++;
		current = current->next;
	}
	return -1;
}

void create(linked_list_t *posts_list, int index, char *name, int user_id)
{
		//initializam postarea
		node_post_t *new_post = init_post(index, name, user_id);
		ll_node_t *new_node = calloc(1, sizeof(ll_node_t));
		new_node->data = new_post;

		if (!posts_list->head) {
			posts_list->head = new_node;
		} else {
			ll_node_t *current = posts_list->head;
			while (current->next)
				current = current->next;
			current->next = new_node;
		}

		posts_list->size++;
		printf("Created %s for %s\n", name, get_user_name(user_id));
		free(name);
}

void adauga_repost1(g_tree_t *events_tree, int repost_id, node_post_t *repost)
{
	g_node_t *repost_father = parcurgere(events_tree, repost_id);
	events_tree->size_tree++;
	repost_father->n_children++;
	g_node_t **aux2 = realloc(repost_father->children,
								(repost_father->n_children + 1) *
								sizeof(g_node_t *));
	repost_father->children = aux2;

	int index = repost_father->n_children - 1;
	repost_father->children[index] = calloc(1, sizeof(g_node_t));
	repost_father->children[index]->value = repost;
	repost_father->children[index]->parent = repost_father;
	repost_father->children[index]->n_children = 0;
	repost_father->children[index]->children =
			calloc(1, sizeof(g_node_t *));
}

void adauga_repost2(g_tree_t *events_tree, node_post_t *repost)
{
	events_tree->size_tree++;
	events_tree->root->n_children++;
	int index = events_tree->root->n_children + 1;
	g_node_t **aux2 =
		realloc(events_tree->root->children,
				index * sizeof(g_node_t *));

	events_tree->root->children = aux2;
	index = index - 2;
	events_tree->root->children[index] = calloc(1, sizeof(g_node_t));
	events_tree->root->children[index]->value = repost;
	events_tree->root->children[index]->parent = events_tree->root;
	events_tree->root->children[index]->n_children = 0;
	events_tree->root->children[index]->children =
			calloc(1, sizeof(g_node_t *));
}

void common_repost(linked_list_t *posts_list, int post_id,
				   int repost_id1, int repost_id2)
{
	node_post_t *post = return_post(posts_list, post_id);
	int *path1 = calloc(MAX_PEOPLE, sizeof(int));
	//calea de la root la repost_id1
	int *path2 = calloc(MAX_PEOPLE, sizeof(int));
	//calea de la root la repost_id2
	int lungime1 = 0;
	int lungime2 = 0;
	g_node_t *repost1 = parcurgere(post->events, repost_id1);
	g_node_t *repost2 = parcurgere(post->events, repost_id2);
	g_node_t *current = repost1;
	while (current != post->events->root) {
		path1[lungime1] = current->value->id;
		lungime1++;
		current = current->parent;
	}
	current = repost2;
	while (current != post->events->root) {
		path2[lungime2] = current->value->id;
		lungime2++;
		current = current->parent;
	}
	path1[lungime1] = post_id;
	path2[lungime2] = post_id;
	lungime1++;
	lungime2++;
	int *inv_path1 = calloc(MAX_PEOPLE, sizeof(int));
	int *inv_path2 = calloc(MAX_PEOPLE, sizeof(int));
	//le inversez pentru a gasi primul nod comun
	for (int i = 0; i < lungime1; i++)
		inv_path1[i] = path1[lungime1 - i - 1];
	for (int i = 0; i < lungime2; i++)
		inv_path2[i] = path2[lungime2 - i - 1];
	int idx_nod_comun = 0;
	while (inv_path1[idx_nod_comun] == inv_path2[idx_nod_comun])
		idx_nod_comun++;

	printf("The first common repost of %d and %d is %d\n",
		   repost_id1, repost_id2, inv_path1[idx_nod_comun - 1]);
	free(path1);
	free(path2);
	free(inv_path1);
	free(inv_path2);
}

void like_repost(linked_list_t *posts_list, int post_id, char *aux,
				 char *user_name)
{
	int user_id = get_user_id(user_name);
	node_post_t *post = return_post(posts_list, post_id);
	char *titlu = post->name;
	int repost_id = atoi(aux);
	g_node_t *repost = parcurgere(post->events, repost_id);
	if (repost->value->post_likes->v[user_id] == 0) {
		repost->value->post_likes->size++;
		repost->value->post_likes->v[user_id] = 1;
		printf("User %s liked repost %s\n", user_name, titlu);
	} else {
		repost->value->post_likes->v[user_id] = 0;
		repost->value->post_likes->size--;
		printf("User %s unliked repost %s\n", user_name, titlu);
	}
}

void like_post(linked_list_t *posts_list, int post_id, char *user_name)
{
	int user_id = get_user_id(user_name);
	node_post_t *post = return_post(posts_list, post_id);
	char *titlu = post->name;
	//verific daca deja a dat like sau nu
	if (post->post_likes->v[user_id] == 0) {
		post->post_likes->size++;
		post->post_likes->v[user_id] = 1;
		printf("User %s liked post %s\n", user_name, titlu);
	} else {
		post->post_likes->v[user_id] = 0;
		post->post_likes->size--;
		printf("User %s unliked post %s\n", user_name, titlu);
	}
}

void ratio(linked_list_t *posts_list, int post_id)
{
	node_post_t *post = return_post(posts_list, post_id);
	int likes = post->post_likes->size;
	int index_repostare = -1;
	caut_like_mare(post->events->root, &index_repostare,
				   likes, post->events->size_tree);
	if (index_repostare == -1)
		printf("The original post is the highest rated\n");
	else
		printf("Post %d got ratio'd by repost %d\n",
			   post_id, index_repostare);
}

void delete_repost(g_tree_t *events_tree, int repost_id)
{
	g_node_t *repost = parcurgere(events_tree, repost_id);
	g_node_t *parinte_repost = repost->parent;
	int n = 0;
	//ma ducl la parinte si scot repostarea din lista de copii
	for (int i = 0; i < parinte_repost->n_children; i++) {
		if (parinte_repost->children[i]->value->id == repost_id) {
			n = i;
			break;
		}
	}
	for (int i = n; i < parinte_repost->n_children; i++)
		parinte_repost->children[i] = parinte_repost->children[i + 1];
	parinte_repost->n_children--;
	//dau free la tot ce e sub repostare, inclusiv ea
	g_node_t ***nodes = free_coarbore(repost, events_tree->size_tree);
	for (int i = 0; i < events_tree->size_tree; i++)
		free(nodes[i]);

	free(nodes);
	printf("Deleted repost #%d of post %s\n", repost_id,
		   events_tree->root->value->name);
}

void delete_post(linked_list_t *posts_list, int post_id)
{
	//dau free la tot arborele si apoi scot nodul din lista de popstari
	node_post_t *post = return_post(posts_list, post_id);
	int n = return_index_lista(posts_list, post_id);
	ll_remove_nth_node(posts_list, n);
	free_post_node(&post);
	printf("Deleted post %d of %s\n", post_id, post->name);
}

void get_likes(linked_list_t *posts_list, char *aux, int post_id)
{
	node_post_t *post = return_post(posts_list, post_id);
	char *titlu = post->name;
	if (aux) {
		int repost_id = atoi(aux);
		g_node_t *repost = parcurgere(post->events, repost_id);
		printf("Repost #%d has %d likes\n",
			   repost->value->id, repost->value->post_likes->size);
	} else {
		printf("Post %s has %d likes\n", titlu, post->post_likes->size);
	}
}

void get_reposts(linked_list_t *posts_list, char *aux, int post_id)
{
	node_post_t *post = return_post(posts_list, post_id);
	if (aux) {
		int repost_id = atoi(aux);
		g_node_t *repost = parcurgere(post->events, repost_id);
		rsd_parcurgere(repost);
	} else {
		rsd_parcurgere(post->events->root);
	}
}

void handle_input_posts(char *input, linked_list_t *posts_list)
{
	char *commands = strdup(input);
	char *cmd = strtok(commands, "\n ");
	static int index = 1;
	if (!cmd)
		return;
	if (!strcmp(cmd, "create")) {
		char *user_name = strtok(NULL, " ");
		int user_id = get_user_id(user_name);
		char *aux = strtok(NULL, "\"");
		char *name = calloc(strlen(aux) + 3, sizeof(char));
		name[0] = '"'; strcat(name, aux); strcat(name, "\"");
		create(posts_list, index, name, user_id);
		index++;
	} else if (!strcmp(cmd, "repost")) {
		char *user_name = strtok(NULL, " ");
		int user_id = get_user_id(user_name);
		int post_id = atoi(strtok(NULL, "\n "));
		node_post_t *repost = init_repost(index, user_id);
		node_post_t *post = return_post(posts_list, post_id);
		char *aux = strtok(NULL, "\n ");
		if (aux) {
			int repost_id = atoi(aux);
			//in caz ca trebuie sa adaugam repostarea la un repost
			adauga_repost1(post->events, repost_id, repost);
		} else {
			//in caz ca trebuie sa adaugam repostarea la o postare
			adauga_repost2(post->events, repost);
		}
		printf("Created repost #%d for %s\n", index, user_name);
		index++;
	} else if (!strcmp(cmd, "common-repost")) {
		int post_id = atoi(strtok(NULL, "\n "));
		int repost_id1 = atoi(strtok(NULL, "\n "));
		int repost_id2 = atoi(strtok(NULL, "\n "));
		common_repost(posts_list, post_id, repost_id1, repost_id2);
	} else if (!strcmp(cmd, "like")) {
		char *user_name = strtok(NULL, " ");
		int post_id = atoi(strtok(NULL, "\n "));
		char *aux = strtok(NULL, "\n ");
		if (aux)
			like_repost(posts_list, post_id, aux, user_name);
		else
			like_post(posts_list, post_id, user_name);

	} else if (!strcmp(cmd, "ratio")) {
		int post_id = atoi(strtok(NULL, "\n "));
		ratio(posts_list, post_id);
	} else if (!strcmp(cmd, "delete")) {
		int post_id = atoi(strtok(NULL, "\n "));
		node_post_t *post = return_post(posts_list, post_id);
		char *aux = strtok(NULL, "\n ");
		if (aux)
			delete_repost(post->events, atoi(aux));
		else
			delete_post(posts_list, post_id);

	} else if (!strcmp(cmd, "get-likes")) {
		int post_id = atoi(strtok(NULL, "\n "));
		char *aux = strtok(NULL, "\n ");
		get_likes(posts_list, aux, post_id);
	} else if (!strcmp(cmd, "get-reposts")) {
		int post_id = atoi(strtok(NULL, "\n "));
		char *aux = strtok(NULL, "\n ");
		get_reposts(posts_list, aux, post_id);
	}
	free(commands);
}
