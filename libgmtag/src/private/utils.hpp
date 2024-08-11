#pragma once

#include <cstddef>

const char *skip_spaces (const char *);
const char *skip_current_line (const char *);
const char *word_into_buffer (const char *, char *, size_t);
const char *
into_buffer_until_newline (const char *, char *, size_t);

inline bool is_number (char i)
{
  return ((i >= '0') && (i <= '9'));
}
