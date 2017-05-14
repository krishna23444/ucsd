/**
 * Authors: Erik Buchanan, Nitay Joffe
 * Class: CSE 123B - Networking
 * Assignment: 1 - Web Server
 * Date: 1/21/07
 */
#include <stdio.h>

#include "CuTest.h"
#include "all_unit_tests.h"

int main(int argc, char **argv) {
  CuString *output = CuStringNew();
  CuSuite *suite = CuSuiteNew();

  CuSuiteAddSuite(suite, htaccess_test_get_suite());
  CuSuiteAddSuite(suite, string_set_test_get_suite());

  CuSuiteRun(suite);
  CuSuiteSummary(suite, output);
  CuSuiteDetails(suite, output);
  printf("%s\n", output->buffer);

  return 0;
}
