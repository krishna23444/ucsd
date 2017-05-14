#ifndef HTACCESS_H__
#define HTACCESS_H__

#include <netinet/in.h>
#include "set.h"

int ip_is_allowed(struct set *ip_exception_set,
                  struct sockaddr_in *client_address);
void parse_htaccess_file(struct set **ip_exception_set,
                         enum deny_or_allow *global_rule);

#endif
