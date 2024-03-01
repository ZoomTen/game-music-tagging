#include "libgmtag.h"
#include <cstdlib>
#include <stdlib.h>
#include <string>
#include <map>

typedef char *(*BufOpFunc) (GmTagDef &, char *);

static char *skip_spaces(char *);
static char *skip_current_line(char *);
static char *word_into_buffer(char *, char *, size_t);
static char *into_buffer_until_newline(char *, char *, size_t);

// map a track number to a GmTagDef
static std::map<uint64_t, GmTagDef> tags;
static std::map<std::string, BufOpFunc> tag_handlers;
static bool tag_handlers_init = false;

static GmTagDef default_tags = {};

// every field has a maximum length of:
#define MAX_FIELD_LENGTH 256

static char *set_album (GmTagDef &, char *);
static char *set_company (GmTagDef &, char *);
static char *set_publisher (GmTagDef &, char *);
static char *set_artist (GmTagDef &, char *);
static char *set_composer (GmTagDef &, char *);
static char *set_sequencer (GmTagDef &, char *);
static char *set_engineer (GmTagDef &, char *);
static char *set_date (GmTagDef &, char *);
static char *set_ripper (GmTagDef &, char *);
static char *set_tagger (GmTagDef &, char *);
static char *set_title (GmTagDef &, char *);
static char *set_length (GmTagDef &, char *);
static char *set_fade (GmTagDef &, char *);
static char *set_comment (GmTagDef &, char *);
static char *set_copyright (GmTagDef &, char *);

void tags_from_buffer(const char *buff) {
    if (!tag_handlers_init) {
        tag_handlers["album"] = set_album;
        tag_handlers["company"] = set_company;
        tag_handlers["publisher"] = set_publisher;
        tag_handlers["artist"] = set_artist;
        tag_handlers["composer"] = set_composer;
        tag_handlers["sequencer"] = set_sequencer;
        tag_handlers["engineer"] = set_engineer;
        tag_handlers["date"] = set_date;
        tag_handlers["ripper"] = set_ripper;
        tag_handlers["tagger"] = set_tagger;
        tag_handlers["title"] = set_title;
        tag_handlers["length"] = set_length;
        tag_handlers["fade"] = set_fade;
        tag_handlers["comment"] = set_comment;
        tag_handlers["copyright"] = set_copyright;

        tag_handlers_init = true;
    }

    // read default tags
    GmTagDef current_tag = default_tags;

    char *buff_pointer = (char *) buff;

    char *line_buffer = (char *) malloc(256);
    line_buffer[255] = '\0';

    char current_sigil;
    char last_sigil;

    while (1) {
        buff_pointer = skip_spaces(buff_pointer);
        if (*buff_pointer == '#') {
            // the meta is stored inside m3u comments
            buff_pointer++;
            buff_pointer = skip_spaces(buff_pointer);
            std::string word;
            
            switch (current_sigil = *buff_pointer++) {
                case '@': // global tag
                case '%': // local tag
                    if (
                        (current_sigil == '%') &&
                        (last_sigil == '@')
                    ){ /*
                        assume this is where the group of
                        "global" tags end and where the
                        "local" tags start, which means there
                        should be no M3U line in between.
                    */
                        default_tags = current_tag;
                    }
                    buff_pointer = word_into_buffer(
                        buff_pointer,
                        line_buffer,
                        255
                    );
                    word = std::string(line_buffer);
                    if (tag_handlers.count(word)) {
                        buff_pointer = tag_handlers[word](current_tag, buff_pointer);
                    }
                    break;
                default:
                    break;
            }

            last_sigil = current_sigil;
        } else {
            // must be an m3u entry, attempt to parse it
            bool next_is_subtune_number = false;
            std::string subtune_number_str = "";
            while (
                (*buff_pointer != '\n') &&
                (*buff_pointer != '\r')
            ){
                if (*buff_pointer == '?') {
                    // in case we have titles like:
                    // "Where the HELL is Carmen Sandiego???.nsf?4"
                    char next_is = *(buff_pointer + (std::ptrdiff_t) 1);
                    if ((next_is >= '0') && (next_is <= '9')) {
                        next_is_subtune_number = true;
                    }
                } else if (next_is_subtune_number) {
                    subtune_number_str.push_back(*buff_pointer);
                }
                buff_pointer++;
            }

            if (subtune_number_str.length() > 0) {
                // we got a subtune number, let's push the current
                // tags into the list
                uint64_t subtune_num = std::stoul(subtune_number_str);
                tags[subtune_num] = current_tag;
                current_tag = default_tags;
            }
        }
        buff_pointer = skip_current_line(buff_pointer);
    }

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
        (*i != '\r') &&
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

static char *set_album (GmTagDef &tag, char *buffer) {
    buffer = skip_spaces(buffer);
    tag.album = (char *) malloc(MAX_FIELD_LENGTH);
    return into_buffer_until_newline(buffer, tag.album, MAX_FIELD_LENGTH);
}

static char *set_title (GmTagDef &tag, char *buffer) {
    buffer = skip_spaces(buffer);
    tag.title = (char *) malloc(MAX_FIELD_LENGTH);
    return into_buffer_until_newline(buffer, tag.title, MAX_FIELD_LENGTH);
}

static char *set_composer (GmTagDef &tag, char *buffer) {
    buffer = skip_spaces(buffer);
    tag.composer = (char *) malloc(MAX_FIELD_LENGTH);
    return into_buffer_until_newline(buffer, tag.composer, MAX_FIELD_LENGTH);
}

// TODO
static char *set_company (GmTagDef &tag, char *buffer) {return buffer;}
static char *set_publisher (GmTagDef &tag, char *buffer) {return buffer;}
static char *set_artist (GmTagDef &tag, char *buffer) {return buffer;}
static char *set_sequencer (GmTagDef &tag, char *buffer) {return buffer;}
static char *set_engineer (GmTagDef &tag, char *buffer) {return buffer;}
static char *set_date (GmTagDef &tag, char *buffer) {return buffer;}
static char *set_ripper (GmTagDef &tag, char *buffer) {return buffer;}
static char *set_tagger (GmTagDef &tag, char *buffer) {return buffer;}
static char *set_length (GmTagDef &tag, char *buffer) {return buffer;}
static char *set_fade (GmTagDef &tag, char *buffer) {return buffer;}
static char *set_comment (GmTagDef &tag, char *buffer) {return buffer;}
static char *set_copyright (GmTagDef &tag, char *buffer) {return buffer;}