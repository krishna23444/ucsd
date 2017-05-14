/**
 * Authors: Erik Buchanan, Nitay Joffe
 * Class: CSE 123B - Networking
 * Assignment: 1 - Web Server
 * Date: 1/21/07
 */
#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <sys/socket.h>

#include "CuTest.h"
#include "htaccess.h"
#include "string_set.h"

static enum deny_or_allow_all global_rule;
static struct string_set *ip_exception_set = NULL;
static FILE *htaccess_file = NULL;

static void setup() {
  global_rule = ALLOW_FROM_ALL;
  if (ip_exception_set != NULL) {
    delete_string_set(ip_exception_set);
    ip_exception_set = NULL;
  }
  if (htaccess_file != NULL) {
    fclose(htaccess_file);
    htaccess_file = NULL;
  }
  htaccess_file = tmpfile();
}

void run_parser() {
  rewind(htaccess_file);
  parse_htaccess_file(htaccess_file, &ip_exception_set, &global_rule);
}

void assert_allow_from_all(CuTest *tc) {
  CuAssertIntEquals(tc, ALLOW_FROM_ALL, global_rule);
}

void assert_deny_from_all(CuTest *tc) {
  CuAssertIntEquals(tc, DENY_FROM_ALL, global_rule);
}

void assert_exception_set_size(CuTest *tc, int size) {
  CuAssertIntEquals(tc, size, size_of_string_set(ip_exception_set));
}

void assert_exception_set_empty(CuTest *tc) {
  assert_exception_set_size(tc, 0);
}

void assert_no_change(CuTest *tc) {
  assert_allow_from_all(tc);
  assert_exception_set_size(tc, 0);
}

void assert_ip_is_allowed(CuTest *tc, const char *address) {
  struct sockaddr_in socket_address;
  inet_aton(address, &socket_address.sin_addr);
  CuAssertTrue(tc, ip_is_allowed(&socket_address, ip_exception_set,
                                 global_rule));
}

void assert_ip_is_denied(CuTest *tc, const char *address) {
  struct sockaddr_in socket_address;
  inet_aton(address, &socket_address.sin_addr);
  CuAssertTrue(tc, !ip_is_allowed(&socket_address, ip_exception_set,
                                  global_rule));
}

void test_empty(CuTest *tc) {
  setup();
  run_parser();
  assert_no_change(tc);
}

void test_allow_all(CuTest *tc) {
  setup();
  fputs("allow from all\n", htaccess_file);
  run_parser();
  assert_no_change(tc);
}

void test_deny_all(CuTest *tc) {
  setup();
  fputs("deny from all\n", htaccess_file);
  run_parser();
  assert_deny_from_all(tc);
  assert_exception_set_empty(tc);
}

void test_mixed_deny_allow(CuTest *tc) {
  setup();
  fputs("deny from all\n", htaccess_file);
  fputs("allow from all\n", htaccess_file);
  fputs("deny from all\n", htaccess_file);
  fputs("allow from all\n", htaccess_file);
  run_parser();
  assert_no_change(tc);
}

void test_deny_with_exceptions(CuTest *tc) {
  setup();
  fputs("deny from all\n", htaccess_file);
  fputs("allow from 192.168.1.1\n", htaccess_file);
  fputs("allow from 127.0.0.1\n", htaccess_file);
  rewind(htaccess_file);

  parse_htaccess_file(htaccess_file, &ip_exception_set, &global_rule);
  assert_deny_from_all(tc);
  assert_exception_set_size(tc, 2);
  assert_ip_is_allowed(tc, "192.168.1.1");
  assert_ip_is_allowed(tc, "127.0.0.1");
  assert_ip_is_denied(tc, "200.140.128.55");
}

CuSuite *htaccess_test_get_suite() {
  CuSuite *suite = CuSuiteNew();
  SUITE_ADD_TEST(suite, test_empty);
  SUITE_ADD_TEST(suite, test_allow_all);
  SUITE_ADD_TEST(suite, test_deny_all);
  SUITE_ADD_TEST(suite, test_mixed_deny_allow);
  SUITE_ADD_TEST(suite, test_deny_with_exceptions);
  return suite;
}
