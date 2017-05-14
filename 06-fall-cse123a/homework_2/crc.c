// Nitay Joffe
// 10/26/2006
// CSE123A - Computer Networks
// Homework 2
// Cyclical Redundancy Checks (CRC)
//
#include <stdio.h>
#include <string.h>

// 0x147 is the generator value converted by hand from 101000111.
static const unsigned int GENERATOR = 0x147;

// The amount of bits we need to shift the message by to make room for the
// remainder. Equal to the number of bits in the generator minus one.
static const unsigned int SHIFT_SIZE = 8;

// Extracts a single bit from an array of bytes. The location to extract from
// spans the entire byte array, meaning the byte array is treated as one big
// contiguous number.
//
// Args:
// - bit_location: integer, location of bit to retrieve in entire array.
// - byte_buffer: [0xAC, 0xBF, 0x04 ...], array of bytes to extract from.
//
// Returns:
// - unsigned integer, 1 if bit is set, 0 otherwise.
//
unsigned int get_bit_from_byte_buffer(unsigned int bit_location,
                                      const unsigned char byte_buffer[]) {
  unsigned int byte_containing_bit = byte_buffer[bit_location / 8];
  unsigned int bitmask = 0x80 >> (bit_location % 8);

  return (byte_containing_bit & bitmask) > 0;
}

// Sets a bit in an array of bytes.
//
// Args:
// - bit_location: integer, location of bit to set, spans across entire buffer.
// - bit_value: 0 or 1, value to set.
// - byte_buffer: [0xAC, 0xBF, 0x04 ...], array of bytes to set bit in.
//
void set_bit_in_byte_buffer(unsigned int bit_location, unsigned char bit_value,
                            unsigned char byte_buffer[]) {
  unsigned int byte_location = bit_location / 8;
  unsigned int bitmask = 0x80 >> (bit_location % 8);

  if (bit_value == 1)
    byte_buffer[byte_location] |= bitmask;
  else
    byte_buffer[byte_location] &= ~bitmask;
}

// Toggles a bit in an array of bytes.
//
// Args:
// - bit_location: integer, location of bit to flip, spans across entire buffer.
// - byte_buffer: [0xAC, 0xBF, 0x04 ...], array of bytes use.
//
void flip_bit_in_byte_buffer(unsigned int bit_location,
                             unsigned char byte_buffer[]) {
  // get current value of bit in byte buffer.
  unsigned int bit_value = get_bit_from_byte_buffer(bit_location, byte_buffer);

  if (bit_value == 0)
    set_bit_in_byte_buffer(bit_location, 1, byte_buffer);
  if (bit_value == 1)
    set_bit_in_byte_buffer(bit_location, 0, byte_buffer);
}

// Check if the bit in the passed in remainder at the location of the most
// significant digit of the generator is set. In other words, check if the bit
// at location SHIFT_SIZE from the right (starting at zero) is set. This is
// used to tell whether a division using exclusive or should occur.
//
// Args:
// - remainder_byte: 0x0FC, 0x13D ... (SHIFT_SIZE+1) sized value to check.
//
// Returns:
// - 1 if bit at SHIFT_SIZE from right in remainder_byte is set, 0 otherwise.
//
unsigned int highest_remainder_bit_is_a_one(unsigned int remainder_byte) {
  return (remainder_byte & (0x1 << SHIFT_SIZE)) > 0;
}

// Print an array of bytes in binary to standard output without any delimitors
// between digits. This effectively concatenates the bytes in the array into
// one binary number.
//
// Args:
// - length_in_bits: 14, 52, 0 ... How many bits are in the array of bytes.
// - byte_buffer: [0xAC, 0xBF, 0x04 ...], array of bytes to print out.
//
void print_byte_buffer_in_binary(unsigned int length_in_bits,
                                 const unsigned char byte_buffer[]) {
  int i;
  for (i = 0; i < length_in_bits; ++i)
    printf("%d",  get_bit_from_byte_buffer(i, byte_buffer));
  printf("\n");
}

// Compute 2r+k, where k is the next bit to be shifted in from the message, and
// r is the current remainder. This handles the shifting in of values when
// doing binary division.
//
// Args:
// - remainder_byte: 0x2D, 0x5E ... pointer to current remainder.
// - next_bit_location: 23, 56 ... where in message to grab next bit from.
// - message_bytes: [0xAC, 0xBF, 0x04 ...], message in array of bytes.
//
void shift_in_next_bit_from_message(unsigned int *remainder_byte,
                                    unsigned int next_bit_location,
                                    unsigned char message_bytes[]) {
  // shift remainder left by 1, same as multiplying by 2.
  *remainder_byte <<= 1;
  // shift in next bit and add to remainder.
  *remainder_byte += get_bit_from_byte_buffer(next_bit_location, message_bytes);
  // zero out all bits beyond size of generator
  *remainder_byte &= 0x1FF;
}

// Add remainder to beginning of message. It is assumed that there has already
// been space allocated for the remainder, and this function will overwrite
// that space.
//
// Args:
// - remainder_byte: 0x34, 0xFD ... remainder to insert.
// - message_bytes: [0xAC, 0xBF, 0x04, 0x00 ...], message in array of bytes.
// - message_length_in_bits: 45, 86 ... number of bits in the message.
//
void insert_remainder_into_message(unsigned int remainder_byte,
                                   unsigned char message_bytes[],
                                   unsigned int message_length_in_bits) {
  unsigned int bitmask = 0x1;
  unsigned int remainder_bit = 0;
  unsigned int i;

  for (i = 0; i < SHIFT_SIZE; ++i) {
    // grab i-th bit in remainder.
    remainder_bit = (remainder_byte & (bitmask << i)) >> i;
    // set i-th bit from the end in message to bit retreived from remainder.
    set_bit_in_byte_buffer(message_length_in_bits - i - 1,
                           remainder_bit, message_bytes);
  }
}

// Compute remainder leftover when dividing binary number by generator using
// mod-2 division (exclusive ors). The binary number is represented by an array
// of bytes. The generator is hardcoded
//
// Args:
// - message_length_in_bits: 45, 87 ... how many bits are in the array of bytes.
// - message_bytes: [0xAC, 0xBF, 0x04, 0x00 ...], message in array of bytes.
//
// Returns:
// - 0x45, 0xF2 ... remainder left over when dividing message by generator.
//
unsigned int compute_remainder(unsigned int message_length_in_bits,
                               unsigned char message_bytes[]) {
  unsigned int remainder_byte = 0x00;
  unsigned int current_bit_location = 0;

  // loop until we reach the end of the message.
  while (current_bit_location < message_length_in_bits) {
    shift_in_next_bit_from_message(&remainder_byte, current_bit_location,
                                   message_bytes);

    if (highest_remainder_bit_is_a_one(remainder_byte))
      // compute new remainder using mod-2 (exclusive or) division.
      remainder_byte ^= GENERATOR;

    // move to next bit in message.
    ++current_bit_location;
  }

  return remainder_byte;
}

// Shifts message to make space for remainder. Computes remainder leftover when
// dividing message by the generator. Inserts remainder into message at space
// reserved in the beginning. This method creates the checksum.
//
// Args:
// - message_length_in_bits: 45, 87 ... pointer to how many bits in message.
// - message_bytes: [0xAC, 0xBF, 0x04, 0x00 ...], message in array of bytes.
//
void sender(unsigned int *message_length_in_bits,
            unsigned char message_bytes[]) {
  unsigned int remainder_byte;

  // shift message to make room for remainder
  *message_length_in_bits += SHIFT_SIZE;

  remainder_byte = compute_remainder(*message_length_in_bits, message_bytes);

  insert_remainder_into_message(remainder_byte, message_bytes,
                                *message_length_in_bits);
}

// Calculates remainder of message divided by generator using mod-2 division
// (exclusive ors), and checks if it is zero. The message is an array of bytes.
// This method verifies the checksum.
//
// Args:
// - message_length_in_bits: 45, 87 ... how many bits are in the array of bytes.
// - message_bytes: [0xAC, 0xBF, 0x04, 0x00 ...], message in array of bytes.
//
// Returns:
// - 1 if remainder of division is zero, 0 otherwise.
//
unsigned int receiver(unsigned int message_length_in_bits,
                      unsigned char message_bytes[]) {
  return compute_remainder(message_length_in_bits, message_bytes);
}

// Brute force every combination of 10 bit burst errors starting at bit zero to
// figure out which ones this generator cannot catch. Each burst error changes
// bit 0, bit 10, and any number of bits in between.
//
// Args:
// - message_length_in_bits: 45, 87 ... how many bits are in the array of bytes.
// - message_bytes: [0xAC, 0xBF, 0x04, 0x00 ...], message in array of bytes.
//
void burst_error_test(unsigned int *message_length_in_bits,
                      unsigned char message_bytes[]) {
  // Create a new buffer for each test so we don't overwrite original data.
  unsigned char erroneous_message_bytes[BUFSIZ];
  unsigned int erroneous_message_length_in_bits;

  unsigned int burst_error_middle;

  unsigned int i;

  for (burst_error_middle = 0; burst_error_middle < 256; ++burst_error_middle) {
    // Reset byte buffer and bit length.
    memcpy(erroneous_message_bytes, message_bytes, BUFSIZ);
    erroneous_message_length_in_bits = *message_length_in_bits;

    // Compute CRC using sender.
    sender(&erroneous_message_length_in_bits, erroneous_message_bytes);

    // Toggle first and tenth bit of burst error.
    flip_bit_in_byte_buffer(0, erroneous_message_bytes);
    flip_bit_in_byte_buffer(9, erroneous_message_bytes);

    // Toggle rest of bits in burst error depending on current value.
    for (i = 0; i < 8; ++i)
      if ((burst_error_middle & (0x80 >> i)) > 0)
        flip_bit_in_byte_buffer(i + 1, erroneous_message_bytes);

    // Run receiver to check new CRC.
    if (receiver(erroneous_message_length_in_bits,erroneous_message_bytes) == 0)
      printf("0x%02x\n", burst_error_middle);
  }
}

// Convert a string of bit characters into an array of bytes.
// Provided from assignment's website.
//
void pack(const char *input, unsigned char *bit_stuffed_input)
{
  int i,j;
  int len = strlen(input);

  for (i = 0; i < len; i++) {
    // x >> 3  divides x by 8
    // x & 0x07 finds remainder when divided by 8
    // x |= (0x80 >> y) sets y_th bit position (from MSB) of x to 1
    // x &= (~(0x80 >> y)) sets y_th bit position (from MSB) of x to 0

    if (input[i] == '1') // set position i to 1
      bit_stuffed_input[i>>3] |= (0x80 >> (i & 0x07));

    else if (input[i] == '0') // set position i to 0
      bit_stuffed_input[i>>3] &= ~(0x80 >> (i & 0x07));
  }

  // Zero out the rest of the last byte. Sorry, this part is a little messy.
  for (i = ((len-1) & 0x07)+1, j = ((len-1) >> 3); i < 8; i++)
    bit_stuffed_input[j] &= ~(0x80 >> i);
}

// Reads string from input file. Converts read string to byte representation.
// Writes byte representation into byte buffer passed in. Returns number of
// bits in the converted message.
//
// Args:
// - message_bytes: [_, _, _ ...], space to store message in byte form at.
//
// Returns:
// - 45, 67 ... number of bits in the message written to byte buffer.
//
unsigned int read_input_and_convert_to_bytes(unsigned char message_bytes[]) {
  unsigned int message_length_in_bits = 0;
  unsigned char message_string_buffer[BUFSIZ];

  // read entire string into buffer, remove newline character at the end.
  fgets(message_string_buffer, BUFSIZ, stdin);

  // set number of bits in the byte message, which is the same as the
  // number of characters in the original string message read from the file.
  message_length_in_bits = strlen(message_string_buffer);

  // remove trailing newline by replacing it with a NULL character.
  if (message_string_buffer[message_length_in_bits - 1] == '\n') {
    message_string_buffer[message_length_in_bits - 1] = '\0';
    --message_length_in_bits;
  }

  // convert string to byte representation
  pack(message_string_buffer, message_bytes);

  return message_length_in_bits;
}

// Driver for program. Runs sender or receiver or burst error test depending on
// the name of the executable that was used. Reads from standard input and
// writes to standard output. Displays checksum computation result for sender
// program, checksum pass/fail for receiver, and burst error middle portion
// bytes that failed checksum test for burst error test.
//
// Args:
// - argc: integer, number of command line arguments passed in.
// - argv: array of strings, command line arguments.
//
// Returns:
// - integer, exit status, 0 if program succeeded.
//
int main(int argc, char* argv[]) {
  // message string read from stdin, converted to byte array
  unsigned char message_bytes[BUFSIZ] = "";
  unsigned int message_length_in_bits = 0;

  message_length_in_bits = read_input_and_convert_to_bytes(message_bytes);

  // sender program, compute CRC
  if (strstr(argv[0], "sender") != NULL) {
    sender(&message_length_in_bits, message_bytes);

    print_byte_buffer_in_binary(message_length_in_bits, message_bytes);
  }

  // receiver program, check if CRC is valid
  else if (strstr(argv[0], "receiver") != NULL) {
    unsigned int remainder_byte;

    remainder_byte = receiver(message_length_in_bits, message_bytes);

    if (remainder_byte == 0)
      printf("CRC check PASSED!\n");
    else
      printf("CRC check FAILED! remainder = 0x%02x\n", remainder_byte);
  }

  // burst error test program, loop through and test all 10 bit burst errors
  else if (strstr(argv[0], "burst_error_test") != NULL)
    burst_error_test(&message_length_in_bits, message_bytes);

  else {
    fprintf(stderr, "USAGE: <sender|receiver> [< <input_file>]\n");
    return 1;
  }

  return 0;
}
