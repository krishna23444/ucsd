/**
 * Authors: Erik Buchanan, Nitay Joffe
 * Class: CSE 123B - Networking
 * Assignment: 1 - Web Server
 * Date: 1/21/07
 */
#ifndef HTTP_H_
#define HTTP_H_

struct http_header {
  const char *version;
  const char *file_path;
};

int parse_http_request(const char* request, struct http_header* header);

#endif
