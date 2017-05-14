/**
 * Authors: Erik Buchanan, Nitay Joffe
 * Class: CSE 123B - Networking
 * Assignment: 1 - Web Server
 * Date: 1/21/07
 */
#include <stdlib.h>
#include <string.h>

#include "debug.h"
#include "string_set.h"

struct string_set *create_string_set() {
  struct string_set *the_string_set = malloc(sizeof(struct string_set));

  the_string_set->size = 0;
  the_string_set->data = malloc(0);

  return the_string_set;
}

// if we used hashing, this could be rewritten in O(1), but we chose to use
// regular old string comparison and keep the set in sorted order O(n).
int find_location_to_insert(struct string_set *the_string_set,
                            const char *message) {
  int message_length = strlen(message);;
  int compare_length;
  int compare_result;
  int i;

  for (i = 0; i < the_string_set->size; ++i) {
    compare_length = strlen(the_string_set->data[i]);
    if (compare_length < message_length)
      compare_length = message_length;

    compare_result = strncmp(the_string_set->data[i], message, compare_length);

    if (compare_result == 0)
      return -1;

    if (compare_result > 0)
      break;
  }

  return i;
}

void add_to_string_set(struct string_set *the_string_set, const char *message) {
  int insert_location = 0;
  int i;

  insert_location = find_location_to_insert(the_string_set, message);
  if (insert_location == -1)
    return;

  the_string_set->size++;
  if (realloc(the_string_set->data,
              sizeof(char *) * the_string_set->size) == NULL)
    return;

  for (i = the_string_set->size - 1; i > insert_location; --i)
    the_string_set->data[i] = the_string_set->data[i-1];

  the_string_set->data[insert_location] = malloc(strlen(message) + 1);
  strncpy(the_string_set->data[insert_location], message, strlen(message) + 1);
}

int is_in_string_set(struct string_set* the_string_set, const char* message) {
  return find_location_to_insert(the_string_set, message) == -1;
}

int size_of_string_set(struct string_set *the_string_set) {
  return the_string_set->size;
}

void clear_string_set(struct string_set *the_string_set) {
  int i;

  for (i = 0; i < the_string_set->size; ++i)
    free(the_string_set->data[i]);

  the_string_set->size = 0;
}

void delete_string_set(struct string_set *the_string_set) {
  int i;

  for (i = 0; i < the_string_set->size; ++i)
    free(the_string_set->data[i]);

  the_string_set->size = 0;
  free(the_string_set->data);
}
