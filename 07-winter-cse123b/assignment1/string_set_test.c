/**
 * Authors: Erik Buchanan, Nitay Joffe
 * Class: CSE 123B - Networking
 * Assignment: 1 - Web Server
 * Date: 1/21/07
 */
#include <stdio.h>

#include "CuTest.h"
#include "string_set.h"

static struct string_set *the_set = NULL;

void setup() {
  if (the_set != NULL) {
    delete_string_set(the_set);
    the_set = NULL;
  }
  the_set = create_string_set();
}

void assert_set_size(CuTest *tc, int size) {
  CuAssertIntEquals(tc, 2, size_of_string_set(the_set));
}

void assert_in_set(CuTest *tc, const char *message) {
  CuAssertTrue(tc, is_in_string_set(the_set, message));
}

void assert_not_in_set(CuTest *tc, const char *message) {
  CuAssertTrue(tc, !is_in_string_set(the_set, message));
}

void test_insert(CuTest *tc) {
  setup();
  add_to_string_set(the_set, "foo");
  add_to_string_set(the_set, "bar");
  assert_set_size(tc, 2);
  assert_in_set(tc, "foo");
  assert_in_set(tc, "bar");
  assert_not_in_set(tc, "Bar");
  assert_not_in_set(tc, "cat");
  assert_not_in_set(tc, "foO");
  assert_not_in_set(tc, "zoo");
}

CuSuite *string_set_test_get_suite() {
  CuSuite *suite = CuSuiteNew();
  SUITE_ADD_TEST(suite, test_insert);
  return suite;
}
