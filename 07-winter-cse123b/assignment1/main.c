/**
 * Authors: Erik Buchanan, Nitay Joffe
 * Class: CSE 123B - Networking
 * Assignment: 1 - Web Server
 * Date: 1/21/07
 */
#include <arpa/inet.h>
#include <errno.h>
#include <netinet/in.h>
#include <pthread.h>
#include <stdarg.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#include "debug.h"
#include "htaccess.h"
#include "server.h"

void usage(const char *program_name, const char* format, ...) {
  va_list ap;
  va_start(ap, format);
  vprintf(format, ap);
  va_end(ap);
  if (strlen(format) > 0)
    printf("\n");
  printf("Usage: %s [-port 8000] [-document_root /home/foo] "
         "[-debug] [-help]\n\n", program_name);
  printf("Where:\n");
  printf("  -port: port number to bind to (default = 80)\n");
  printf("  -document_root: root of website to serve (default = \".\")\n");
  printf("  -debug: turn on loads of debugging information\n");
  printf("  -help: this message\n");
}

int main(int argc, char** argv) {
  unsigned int port_number = 80;

  FILE *htaccess_file = NULL;

  int i;
  int arg_length;

  int server_socket;
  struct sockaddr_in server_address;

  int client_socket;
  struct sockaddr_in client_address;
  int sockaddr_in_size;

  pthread_t connection_thread;
  struct connection_handler_arguments* thread_arguments = NULL;

  enum deny_or_allow_all global_rule = ALLOW_FROM_ALL;
  struct string_set* ip_exception_set = NULL;
  
  char htaccess_location [BUFSIZ];

  debug_disable();

  for (i = 1; i < argc; ++i) {
    arg_length = strlen(argv[i]);
    if (strncmp("-document_root", argv[i], arg_length) == 0) {
      ++i;
      document_root = argv[i];
    } else if (strncmp("-port", argv[i], arg_length) == 0) {
      ++i;
      port_number = atoi(argv[i]);
    } else if (strncmp("-debug", argv[i], arg_length) == 0) {
      debug_enable();
    } else if (strncmp("-help", argv[i], arg_length) == 0) {
      usage(argv[0], "");
      return -1;
    } else {
      usage(argv[0], "Unknown option given: %s", argv[i]);
      return -1;
    }
  }

  dbg("using port %d and document_root %s\n", port_number, document_root);

  server_socket = socket(AF_INET, SOCK_STREAM, 0);
  if (server_socket == -1) {
    perror("creating server socket failed");
    return -1;
  }
  dbg("created server socket\n");

  server_address.sin_family = AF_INET;
  server_address.sin_port = htons(port_number);
  server_address.sin_addr.s_addr = INADDR_ANY;
  memset(&(server_address.sin_zero), 0, 8);
  if (bind(server_socket, (struct sockaddr*)&server_address,
                           sizeof(struct sockaddr)) == -1) {
    perror("binding server to port");
    return -1;
  }
  dbg("server bound to port %d\n", port_number);


  if (listen(server_socket, 10) == -1) {
    perror("listening on server socket");
    return -1;
  };
  dbg("server listening\n");

  snprintf(htaccess_location, sizeof(htaccess_location), "%s/.htaccess",
           document_root);
  htaccess_file = fopen(htaccess_location, "r");
  if (htaccess_file != NULL) {
    parse_htaccess_file(htaccess_file, &ip_exception_set, &global_rule);
    fclose(htaccess_file);
  }

  sockaddr_in_size = sizeof(struct sockaddr_in);
  while (1) {
    client_socket = accept(server_socket, (struct sockaddr*)&client_address,
                           (socklen_t*)&sockaddr_in_size);
    if (client_socket == -1) {
      perror("accepting client connection");
      continue;
    }
    if (!ip_is_allowed(&client_address, ip_exception_set, global_rule)) {
      dbg("rejecting client %s because of htaccess\n",
          inet_ntoa(client_address.sin_addr));
          
          
      send_error(client_socket, 403);
      close(client_socket);
      continue;
    }
    dbg("client connection accepted, forking a thread to handle it\n");

    thread_arguments = malloc(sizeof(struct connection_handler_arguments));
    thread_arguments->client_socket = client_socket;

    /* according to man page, the error number is returned (not set in errno
       automatically), so we have to do it manually */
    errno = pthread_create(&connection_thread, NULL, connection_handler,
                           (void *)thread_arguments);
    if (errno != 0) {
      perror("forking thread to handle client connection");
      continue;
    }
    dbg("forked thread id %d\n", (int)connection_thread);
  }

  close(server_socket);
  if (ip_exception_set != NULL)
    delete_string_set(ip_exception_set);
  return 0;
}
