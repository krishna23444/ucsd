/**
 * Authors: Erik Buchanan, Nitay Joffe
 * Class: CSE 123B - Networking
 * Assignment: 1 - Web Server
 * Date: 1/21/07
 */
#ifndef STRING_SET_H_
#define STRING_SET_H_

struct string_set {
  int size;
  char **data;
};

struct string_set *create_string_set();
void add_to_string_set(struct string_set *the_string_set, const char *message);
int is_in_string_set(struct string_set *the_string_set, const char *message);
int size_of_string_set(struct string_set *the_string_set);
void clear_string_set(struct string_set *the_string_set);
void delete_string_set(struct string_set *the_string_set);

#endif
