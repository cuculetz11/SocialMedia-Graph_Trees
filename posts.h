#ifndef POSTS_H
#define POSTS_H
#define MAX_PEOPLE 550
/**
 * Function that handles the calling of every command from task 2
 *
 * Please add any necessary parameters to the functions
 */
typedef struct g_tree_t g_tree_t;
typedef struct g_node_t g_node_t;
typedef struct node_post_t node_post_t;
struct g_tree_t {
	g_node_t *root;
	int size_tree;
};

struct g_node_t {
	g_node_t *parent;
	//retin nodul parinte
	node_post_t *value;
	g_node_t **children;
	int n_children;
};

typedef struct like_t {
	char *v;
	int size;
} like_t;

typedef struct node_post_t {
	int id;
	char *name;
	int user_id;
	g_tree_t *events;
	like_t *post_likes;

} node_post_t;

typedef struct ll_node_t {
	void *data;
	struct ll_node_t *next;
} ll_node_t;

typedef struct linked_list_t {
	ll_node_t *head;
	int data_size;
	int size;
} linked_list_t;

typedef struct stack_t stack;
struct stack_t {
	struct linked_list_t *list;
};

typedef struct queue_t queue_t;
struct queue_t {
	/* Dimensiunea maxima a cozii */
	unsigned int max_size;
	/* Numarul de elemente din coada */
	unsigned int size;
	/* Dimensiunea in octeti a tipului de date stocat in coada */
	unsigned int data_size;
	/* Indexul de la care se vor efectua operatiile de front si dequeue */
	unsigned int read_idx;
	/* Indexul de la care se vor efectua operatiile de dequeue */
	unsigned int write_idx;
	/* Bufferul ce stocheaza elementele cozii */
	void **buff;
};

void handle_input_posts(char *input, linked_list_t *posts_list);
void free_post_list(linked_list_t *posts_list);

void free_post_node(node_post_t **post);
g_node_t ***free_g_tree(g_tree_t *tree);
queue_t *q_create(unsigned int data_size, unsigned int max_size);
unsigned int q_get_size(queue_t *q);
unsigned int q_is_empty(queue_t *q);
void *q_front(queue_t *q);
int q_dequeue(queue_t *q);
int q_enqueue(queue_t *q, void *new_data);
void q_clear(queue_t *q);
void q_free(queue_t *q);
void q_free_parcurgere(queue_t *q);
int q_enqueue1(queue_t *q, void *new_data);
node_post_t *return_post(linked_list_t *posts_list, int id);

#endif // POSTS_H
