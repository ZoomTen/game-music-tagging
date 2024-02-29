#include "libgmtag.hpp"
#include <stdlib.h>

static char *skip_spaces(char *);
static char *skip_current_line(char *);
static char *word_into_buffer(char *, char *, size_t);
static char *into_buffer_until_newline(char *, char *, size_t);

GmTags::GmTags() {}
GmTags::~GmTags() {}

void GmTags::tags_from_buffer(const char *buff) {
    // read default tags
    char *buff_pointer = (char *) buff;

    char *line_buffer = (char *) malloc(256);
    line_buffer[255] = '\0';

    // while (1) {
        buff_pointer = skip_spaces(buff_pointer);
        if (*buff_pointer == '#') {
            buff_pointer++;
            buff_pointer = into_buffer_until_newline(
                buff_pointer,
                line_buffer,
                255
            );
        } else {
            buff_pointer = skip_current_line(buff_pointer);
        }
    // }

    GmTagDef i = GmTagDef();
    i.album = "AAA";
    GmTagDef j = i;
    j.album = "Overridden!";
    tags[0] = i;
    tags[5] = j;
}

// skip spaces and newlines
char *skip_spaces(char *i) {
    while(
        (*i == ' ') ||
        (*i == '\t') ||
        (*i == '\n') ||
        (*i == '\r')
    ){
        i++;
    }
    return i;
}

char *skip_current_line(char *i) {
    // assumes windows/unix line endings!
    while(
        (*i != '\r') ||
        (*i != '\n')
    ){
        i++;
    }
    return ++i;
}

char *word_into_buffer(char *source, char *buffer, size_t max_size) {
    size_t i = 0;
    while (
        (*source != ' ') &&
        (*source != '\t') &&
        (*source != '\r') &&
        (*source != '\n')
    ){
        if (++i == max_size)
        {
            break;
        }
        *buffer++ = *source++;
    }
    *buffer = '\0';
    return source;
}

char *into_buffer_until_newline(char *source, char *buffer, size_t max_size) {
    size_t i = 0;
    // also assumes windows/unix line endings!
    while ((*source != '\n') && (*source != '\r')){
        if (++i == max_size)
        {
            break;
        }
        *buffer++ = *source++;
    }
    *buffer = '\0';
    return source;
}