/**
 * Authors: Erik Buchanan, Nitay Joffe
 * Class: CSE 123B - Networking
 * Assignment: 1 - Web Server
 * Date: 1/21/07
 */
#include <stdio.h>
#include <string.h>

#include "debug.h"
#include "http.h"

int parse_http_request(const char* request, struct http_header* header) {
  dbg("request:\n%s", request);

  const char* get_location = strstr(request, "GET");
  if (get_location == NULL) {
    fprintf(stderr, "could not find GET in request\n");
    return -1;
  }

  const char* file_path_location = get_location + strlen("GET ");

  char* end_of_filename = strchr(file_path_location, ' ');
  if (end_of_filename == NULL) {
    dbg("could not find end of file name\n");
    return -1;
  }
  *end_of_filename = '\0';

  const char* http_version_location = end_of_filename + 1;
  if (strncmp("HTTP/", http_version_location, strlen("HTTP/")) != 0) {
    dbg("cannot find HTTP version number\n");
    return -1;
  }

  char* newline_location = strstr(http_version_location, "\r\n");
  if (newline_location == NULL) {
    dbg("cannot find end of HTTP version number\n");
    return -1;
  }
  *newline_location = '\0';

  header->file_path = file_path_location;
  header->version = http_version_location + strlen("HTTP/");

  if (strncmp(header->version, "1.1", strlen("1.1")) == 0) {
    char *host_location = strstr(newline_location + 1, "Host");

    if (host_location == NULL)
      return -1;
  }

  return 0;
}
