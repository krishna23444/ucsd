/**
 * Remove newlines from a string.
 *
 * Args:
 * - string_buffer: string to remove newlines from. Will be altered after call.
 */
void remove_newlines(char *string_buffer) {
  // next location in string to read from
  char *read_pointer = string_buffer;
  // next location in string to write to
  char *write_pointer = string_buffer;

  while (*read_pointer != '\0') {
    // skip over character if it is a newline
    if (*read_pointer != '\n') {
      // copy character, increment write_pointer to next character
      *write_pointer = *read_pointer;
      ++write_pointer;
    }

    // move to next character to read
    ++read_pointer;
  }

  // mark the end of the string without newlines
  *write_pointer = '\0';
}
