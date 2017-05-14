/**
 * Authors: Erik Buchanan, Nitay Joffe
 * Class: CSE 123B - Networking
 * Assignment: 1 - Web Server
 * Date: 1/21/07
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <errno.h>
#include <unistd.h>
#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <sys/stat.h>

#include "debug.h"
#include "htaccess.h"
#include "http.h"
#include "server.h"
#include "string_set.h"

static unsigned int number_of_active_connections = 0;
static pthread_mutex_t active_connections_lock;

const char* document_root = ".";

#define RESPONSE_OK 200
#define RESPONSE_MALFORMED 400
#define RESPONSE_PERMISSION_ERROR 403
#define RESPONSE_FILE_NOT_FOUND_ERROR 404

struct server_response {
  unsigned int response_code;
  int content_length;
  const char *http_version;
  char content_type[20];
};

int get_wait_time_in_seconds() {
  const int MAX_TIME_IN_SECONDS = 120;
  const int DROPOFF_TIME_IN_SECONDS = 10;

  int wait_time = MAX_TIME_IN_SECONDS - DROPOFF_TIME_IN_SECONDS *
                  (number_of_active_connections - 1);
  return wait_time > 0 ? wait_time : 0;
}

int get_file_size (char *document_path) {
  struct stat info;

  if (stat(document_path, &info) == -1) {
    perror("stat failed");
    return 0;
  }

  return info.st_size;
}

void get_content_type(const char *fileName, char *buffer) {
  char *extension = strrchr(fileName, '.') + 1;

  if (strncasecmp(extension, "html", strlen("html")) == 0)
    strncpy(buffer, "text/html", strlen("text/html"));
  else if (strncasecmp(extension, "htm", strlen("htm")) == 0)
    strncpy(buffer, "text/html", strlen("text/html"));
  else if (strncasecmp(extension, "jpg", strlen("jpg")) == 0)
    strncpy(buffer, "image/jpg", strlen("image/jpg"));
  else if (strncasecmp(extension, "jpeg", strlen("jpeg")) == 0)
    strncpy(buffer, "image/jpg", strlen("image/jpg"));
  else if (strncasecmp(extension, "gif", strlen("gif")) == 0)
    strncpy(buffer, "image/gif", strlen("image/gif"));
  else if (strncasecmp(extension, "css", strlen("css")) == 0)
    strncpy(buffer, "test/css",strlen("test/css"));
  else if (strncasecmp(extension, "js", strlen("js")) == 0)
    strncpy(buffer, "application/x-javascript",
            strlen("application/x-javascript"));
  else if (strncasecmp(extension, "txt", strlen("txt")) == 0)
    strncpy(buffer, "text/txt", strlen("text/txt"));
  else
    strncpy(buffer, "", strlen(""));
}

void send_to_client(int client_socket, char *message, int size) {
  int number_of_bytes_left_to_write = size;
  int number_of_bytes_written = 0;
  
  if (size < 30)
    dbg("%s", message);

  while (number_of_bytes_left_to_write > 0) {
    number_of_bytes_written = write(client_socket, message,
                                    number_of_bytes_left_to_write);

    if (number_of_bytes_written <= 0) {
      fprintf(stderr, "FAILED to send file\n");
      return;
    }

    message += number_of_bytes_written;
    number_of_bytes_left_to_write -= number_of_bytes_written;
  }

  dbg(">> %d bytes\n", size);
}

void send_document(int client_socket, FILE *document) {
  char send_buffer[BUFSIZ];
  int number_of_bytes_read;

  number_of_bytes_read = fread(send_buffer, 1, sizeof(send_buffer), document);
  while (number_of_bytes_read > 0) {
    send_to_client(client_socket, send_buffer, number_of_bytes_read);
    number_of_bytes_read = fread(send_buffer, 1, sizeof(send_buffer), document);
  }
}

void send_header_information(int client_socket,
                             const struct server_response *header_info) {
  char msg_buffer[BUFSIZ] = "";

  snprintf(msg_buffer, sizeof(msg_buffer), "HTTP/%s ",
           header_info->http_version);

  send_to_client(client_socket, msg_buffer, strlen(msg_buffer));

  send_to_client(client_socket, "200 OK\r\n", strlen("200 OK\r\n"));

  snprintf(msg_buffer, sizeof(msg_buffer), "Content-Type: %s\r\n",
           header_info->content_type);
  send_to_client(client_socket, msg_buffer, strlen(msg_buffer));

  snprintf(msg_buffer, sizeof(msg_buffer), "Content-Length: %d\r\n",
           header_info->content_length);
  send_to_client(client_socket, msg_buffer, strlen(msg_buffer));
  
  send_to_client(client_socket, "\r\n", strlen("\r\n"));
}

void send_error(int client_socket, int error_number) {
  const char const *FILE_NOT_FOUND_ERROR = "404 Not Found\r\n";
  const char const *PERMISSION_ERROR = "403 Permission Denied\r\n";
  const char const *MALFORMED = "400 Bad Request\r\n";

  const char const *CONTENT_TYPE = "Content-Type: text/html\r\n";

  char *message;

  char msg_buffer[BUFSIZ];

  send_to_client(client_socket, "HTTP/1.1 ", strlen("HTTP/1.1 "));

  switch (error_number) {
    case RESPONSE_OK:
      return;
    case RESPONSE_FILE_NOT_FOUND_ERROR:
      message = (char *)FILE_NOT_FOUND_ERROR;
      break;
    case RESPONSE_PERMISSION_ERROR:
      message = (char *)PERMISSION_ERROR;
      break;
    case RESPONSE_MALFORMED:
      message = (char *)MALFORMED;
      break;
  }

  send_to_client(client_socket, message, strlen(message));

  send_to_client(client_socket, (char *)CONTENT_TYPE, strlen(CONTENT_TYPE));

  snprintf(msg_buffer, sizeof(msg_buffer), "Content-Length: %d\r\n",
           (int)strlen(message));

  send_to_client(client_socket, msg_buffer, strlen(msg_buffer));

  send_to_client(client_socket, "\r\n", strlen("\r\n"));

  send_to_client(client_socket, message, strlen(message));
}

void send_client_response(int client_socket, const struct http_header *request_details) {
  char document_path[BUFSIZ];
  FILE* document;
  
  struct server_response header_information;

  dbg("<< GET %s %s\n", request_details->file_path, request_details->version);

  snprintf(document_path, sizeof(document_path), "%s/%s", document_root,
           request_details->file_path);
  if (document_path[strlen(document_path) - 1] == '/')
    strncat(document_path, "index.html", sizeof(document_path));

  dbg("retrieving file %s\n", document_path);

  header_information.http_version = request_details->version;

  document = fopen(document_path, "r");
  if (document == NULL) {
    switch (errno) {
      case ENOENT:
        send_error(client_socket, RESPONSE_FILE_NOT_FOUND_ERROR);
        break;
      case EACCES:
        send_error(client_socket, RESPONSE_PERMISSION_ERROR);
        break;
    }
    dbg("error %d reading document %s: %s\n", errno, document_path,
        strerror(errno));
    return;
  }

  get_content_type(document_path, header_information.content_type);
  header_information.content_length = get_file_size(document_path);
  
  send_header_information(client_socket, &header_information);
  send_document(client_socket, document);
}

void *connection_handler(void* arg) {
  int self_id = (int)pthread_self();

  struct connection_handler_arguments* thread_arguments = (struct connection_handler_arguments*)arg;
  int client_socket = thread_arguments->client_socket;

  char request[BUFSIZ] = "";
  int number_of_bytes_read = 0;

  struct http_header request_details;
  int done = 0;

  struct timeval time_to_wait;
  time_to_wait.tv_usec = 0;
  fd_set read_fd_set;
  int select_return_value;

  pthread_mutex_lock(&active_connections_lock);
  number_of_active_connections++;
  pthread_mutex_unlock(&active_connections_lock);

  while (!done) {
    time_to_wait.tv_sec = get_wait_time_in_seconds();

    FD_ZERO(&read_fd_set);
    FD_SET(client_socket, &read_fd_set);
    select_return_value = select(client_socket+1, &read_fd_set, NULL, NULL, &time_to_wait);
    
    if (select_return_value <= 0) {
      if (select_return_value == -1)
        perror("select error");
      break;
    }

    number_of_bytes_read = read(client_socket, request, sizeof(request));
    if (number_of_bytes_read == -1) {
      perror("reading request message from client");
      return (void *)-1;
    }
    dbg("<< (%d) %d bytes\n", self_id, number_of_bytes_read);

    if (parse_http_request(request, &request_details) == 0) {
      send_client_response(client_socket, &request_details);
      done = (strncmp(request_details.version, "1.1", strlen("1.1")) != 0);
    }
    else {
      send_error(client_socket, RESPONSE_MALFORMED);
      done = 1;
    }
  }

  pthread_mutex_lock(&active_connections_lock);
  number_of_active_connections--;
  pthread_mutex_unlock(&active_connections_lock);

  close(client_socket);
  free(thread_arguments);
  dbg("thread id %d exiting\n", self_id);
  return (void *)0;
}
