/**
 * Authors: Erik Buchanan, Nitay Joffe
 * Class: CSE 123B - Networking
 * Assignment: 1 - Web Server
 * Date: 1/21/07
 */
#ifndef SERVER_H_
#define SERVER_H_

#include <pthread.h>

struct connection_handler_arguments {
  int client_socket;
};

extern const char* document_root;

void *connection_handler(void *arguments);
void send_error(int client_socket, int error_number);

#endif
