#ifndef FRIENDS_H
#define FRIENDS_H

#define MAX_COMMAND_LEN 500
#define MAX_PEOPLE 550

#include <stdint.h>
void suggestion_helper(char *name, int ***friends);
void common_helper(char *name1, char *name2, int ***friends);
int friends_count(uint16_t id, int ***friends);
int distance_helper(uint16_t id1, uint16_t id2, int ***friends);
void popular_helper(char *name, int ***friends);
/**
 * Function that handles the calling of every command from task 1
 *
 * Please add any necessary parameters to the functions
 */
void handle_input_friends(char *input, int ***friends);

#endif // FRIENDS_H
