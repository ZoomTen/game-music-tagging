#include <cstddef>

char *skip_spaces (char *);
char *skip_current_line (char *);
char *word_into_buffer (char *, char *, size_t);
char *into_buffer_until_newline (char *, char *, size_t);

inline bool is_number (char i) {
    return ((i >= '0') && (i <= '9'));
}
