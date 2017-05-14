void print_buffer_of_bytes(unsigned char buffer[], unsigned int bit_count) {
  int i = 0;
  int byte_count = bit_count / 8;
  int extra_bits_count = 8 - bit_count % 8;

  if (extra_bits_count > 0)
    ++byte_count;

  for (i = 0; i < byte_count; ++i)
    printf("0x%02x ", buffer[i]);

  if (extra_bits_count > 0)
    printf("(%d extra zeroes added)", extra_bits_count);

  printf("\n");
}
