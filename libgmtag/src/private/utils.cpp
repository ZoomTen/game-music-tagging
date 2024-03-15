#include "./utils.hpp"

// skip spaces
char *skip_spaces (char *i) {
    while ((*i == ' ') || (*i == '\t')) {
        i++;
    }
    return i;
}

char *skip_current_line (char *i) {
    char current_char = *i;

    while ((current_char != '\r') && (current_char != '\n') &&
        (current_char != '\0'))
    {
        i++;
        current_char = *i;
    }
    switch (current_char) {
        case '\0':
            // if EOF, just return the end addr
            return i;
        case '\r':
            // dos/windows line ending
            i++;  // always assume \n, classic mac discounted
            return ++i;
        case '\n':
        default:
            return ++i;
    }
}

char *
word_into_buffer (char *source, char *buffer, size_t max_size) {
    size_t i = 0;
    while ((*source != ' ') && (*source != '\t') &&
        (*source != '\r') && (*source != '\n'))
    {
        if (++i == max_size) {
            break;
        }
        *buffer++ = *source++;
    }
    *buffer = '\0';
    return source;
}

char *into_buffer_until_newline (
    char *source,
    char *buffer,
    size_t max_size
) {
    size_t i = 0;
    // also assumes windows/unix line endings!
    while ((*source != '\n') && (*source != '\r')) {
        if (++i == max_size) {
            break;
        }
        *buffer++ = *source++;
    }
    *buffer = '\0';
    return source;
}
