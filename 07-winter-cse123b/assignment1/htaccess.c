/**
 * Authors: Erik Buchanan, Nitay Joffe
 * Class: CSE 123B - Networking
 * Assignment: 1 - Web Server
 * Date: 1/21/07
 */
#include <arpa/inet.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stdio.h>
#include <string.h>
#include <strings.h>
#include <sys/socket.h>

#include "debug.h"
#include "htaccess.h"
#include "string_set.h"

void parse_htaccess_file(FILE *htaccess_file,
                         struct string_set **ip_exception_string_set,
                         enum deny_or_allow_all *global_rule) {
  char line[BUFSIZ];

  char *deny_or_allow;
  int allow;
  char *from;
  char *ip_or_hostname_or_all;
  
  struct hostent* host_entry;

  *ip_exception_string_set = create_string_set();
  while (fgets(line, sizeof(line), htaccess_file) != NULL) {
    deny_or_allow = strtok(line, " ");
    from = strtok(NULL, " ");
    ip_or_hostname_or_all = strtok(NULL, " \n");

    if (strncasecmp("allow", deny_or_allow, strlen("allow")) == 0) {
      allow = 1;
      dbg("allow ");
    }
    else if (strncasecmp("deny", deny_or_allow, strlen("deny")) == 0) {
      allow = 0;
      dbg("deny ");
    }
    else {
      dbg("error on deny/allow on line: %s\n", line);
      continue;
    }

    dbg("from ");

    if (strncasecmp("all", ip_or_hostname_or_all, strlen("all")) == 0) {
      clear_string_set(*ip_exception_string_set);
      *global_rule = allow ? ALLOW_FROM_ALL : DENY_FROM_ALL;
      dbg("all\n");
    } else {
      host_entry = gethostbyname(ip_or_hostname_or_all);
      if (host_entry == NULL) {
        dbg("error looking up %s\n", ip_or_hostname_or_all);
        return;
      }
      dbg("--%s--\n", host_entry->h_name);
      add_to_string_set(*ip_exception_string_set, host_entry->h_name);
    }
  }
}

int ip_is_allowed(struct sockaddr_in *client_address,
                  struct string_set *ip_exception_string_set,
                  enum deny_or_allow_all global_rule) {
  const char* ip_address;
  int ip_is_in_string_set;
  
  if (ip_exception_string_set == NULL)
    return global_rule == ALLOW_FROM_ALL;

  ip_address = inet_ntoa(client_address->sin_addr);
  ip_is_in_string_set = is_in_string_set(ip_exception_string_set, ip_address);

  return global_rule == DENY_FROM_ALL ? ip_is_in_string_set : !ip_is_in_string_set;
}
