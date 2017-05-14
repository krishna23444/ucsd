/**
 * Authors: Erik Buchanan, Nitay Joffe
 * Class: CSE 123B - Networking
 * Assignment: 1 - Web Server
 * Date: 1/21/07
 */
#ifndef HTACCESS_H_
#define HTACCESS_H_

#include <stdio.h>
#include <netinet/in.h>
#include "string_set.h"

enum deny_or_allow_all { ALLOW_FROM_ALL, DENY_FROM_ALL };

void parse_htaccess_file(FILE *htaccess_file,
                         struct string_set **ip_exception_set,
                         enum deny_or_allow_all *global_rule);
int ip_is_allowed(struct sockaddr_in *client_address,
                  struct string_set *ip_exception_set,
                  enum deny_or_allow_all global_rule);

#endif
