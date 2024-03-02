#include "libgmtag.h"

#include <stdlib.h>
#include <vector>
#include <map>
#include <string>

typedef char *(*BufOpFunc)(GmTagDef &, char *);

static char *skip_spaces (char *);
static char *skip_current_line (char *);
static char *word_into_buffer (char *, char *, size_t);
static char *into_buffer_until_newline (char *, char *, size_t);

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

void tags_from_buffer (char *buff) {
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

  char *buff_pointer = buff;

  char *line_buffer = static_cast<char *>(malloc(256));
  line_buffer[255] = '\0';

  char current_sigil;
  char last_sigil;

  while (true) {
    buff_pointer = skip_spaces(buff_pointer);
    if (*buff_pointer == '#') {
      // the meta is stored inside m3u comments
      buff_pointer++;
      buff_pointer = skip_spaces(buff_pointer);
      std::string word;

      switch (current_sigil = *buff_pointer++) {
        case '@':  // global tag
        case '%':  // local tag
          if ((current_sigil == '%') && (last_sigil == '@'))
          { /*
             assume this is where the group of
             "global" tags end and where the
             "local" tags start.
         */
            default_tags = current_tag;
          }
          buff_pointer =
              word_into_buffer(buff_pointer, line_buffer, 255);

          word = std::string(line_buffer);
          if (tag_handlers.count(word)) {
            buff_pointer =
                tag_handlers[word](current_tag, buff_pointer);
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
      while ((*buff_pointer != '\0') &&
             (*buff_pointer != '\n') && (*buff_pointer != '\r'))
      {
        if (*buff_pointer == '?') {
          // in case we have titles like:
          // "Where the HELL is Carmen
          // Sandiego???.nsf?4"
          char next_is = *(buff_pointer + 1);
          next_is_subtune_number =
              ((next_is >= '0') && (next_is <= '9'));
        } else if (next_is_subtune_number) {
          subtune_number_str.push_back(*buff_pointer);
        }
        buff_pointer++;
      }

      if (subtune_number_str.length() > 0) {
        // we got a subtune number, let's push the
        // current tags into the list
        uint64_t subtune_num = std::stoul(subtune_number_str);
        if (tags.count(subtune_num)) {
          // if a subtune exists, modify its properties
          GmTagDef prev_tag = tags[subtune_num];

          if (current_tag.album)
            prev_tag.album = current_tag.album;
          if (current_tag.company)
            prev_tag.company = current_tag.company;
          if (current_tag.publisher)
            prev_tag.publisher = current_tag.publisher;
          if (current_tag.artist)
            prev_tag.artist = current_tag.artist;
          if (current_tag.composer)
            prev_tag.composer = current_tag.composer;
          if (current_tag.sequencer)
            prev_tag.sequencer = current_tag.sequencer;
          if (current_tag.engineer)
            prev_tag.engineer = current_tag.engineer;
          // if (current_tag.date) prev_tag.date =
          // current_tag.date;
          if (current_tag.ripper)
            prev_tag.ripper = current_tag.ripper;
          if (current_tag.tagger)
            prev_tag.tagger = current_tag.tagger;
          if (current_tag.title)
            prev_tag.title = current_tag.title;
          if (current_tag.comments)
            prev_tag.comments = current_tag.comments;
          if (current_tag.copyright)
            prev_tag.copyright = current_tag.copyright;
          // if (current_tag.length) prev_tag.length =
          // current_tag.length; if (current_tag.fade)
          // prev_tag.fade = current_tag.fade;
          tags[subtune_num] = prev_tag;
        } else {
          tags[subtune_num] = current_tag;
        }
        current_tag = default_tags;
      }
    }
    if (*buff_pointer == '\0')
      break;
    buff_pointer = skip_current_line(buff_pointer);
  }
}

GmTagDef get_tags_for_subtune (unsigned long subtune) {
  if (tags.count(subtune))
    return tags[subtune];
  return default_tags;
}

// skip spaces and newlines
char *skip_spaces (char *i) {
  while ((*i == ' ') || (*i == '\t') || (*i == '\n') ||
         (*i == '\r'))
  {
    i++;
  }
  return i;
}

char *skip_current_line (char *i) {
  // assumes windows/unix line endings!
  while ((*i != '\r') && (*i != '\n')) {
    i++;
  }
  return ++i;
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

static char *set_album (GmTagDef &tag, char *buffer) {
  buffer = skip_spaces(buffer);
  tag.album = static_cast<char *>(malloc(MAX_FIELD_LENGTH));
  return into_buffer_until_newline(
      buffer,
      tag.album,
      MAX_FIELD_LENGTH
  );
}

static char *set_title (GmTagDef &tag, char *buffer) {
  buffer = skip_spaces(buffer);
  tag.title = static_cast<char *>(malloc(MAX_FIELD_LENGTH));
  return into_buffer_until_newline(
      buffer,
      tag.title,
      MAX_FIELD_LENGTH
  );
}

static char *set_composer (GmTagDef &tag, char *buffer) {
  buffer = skip_spaces(buffer);
  tag.composer = static_cast<char *>(malloc(MAX_FIELD_LENGTH));
  return into_buffer_until_newline(
      buffer,
      tag.composer,
      MAX_FIELD_LENGTH
  );
}

static char *set_company (GmTagDef &tag, char *buffer) {
  buffer = skip_spaces(buffer);
  tag.company = static_cast<char *>(malloc(MAX_FIELD_LENGTH));
  return into_buffer_until_newline(
      buffer,
      tag.company,
      MAX_FIELD_LENGTH
  );
}

static char *set_publisher (GmTagDef &tag, char *buffer) {
  buffer = skip_spaces(buffer);
  tag.publisher = static_cast<char *>(malloc(MAX_FIELD_LENGTH));
  return into_buffer_until_newline(
      buffer,
      tag.publisher,
      MAX_FIELD_LENGTH
  );
}

static char *set_artist (GmTagDef &tag, char *buffer) {
  buffer = skip_spaces(buffer);
  tag.artist = static_cast<char *>(malloc(MAX_FIELD_LENGTH));
  return into_buffer_until_newline(
      buffer,
      tag.artist,
      MAX_FIELD_LENGTH
  );
}

static char *set_sequencer (GmTagDef &tag, char *buffer) {
  buffer = skip_spaces(buffer);
  tag.sequencer = static_cast<char *>(malloc(MAX_FIELD_LENGTH));
  return into_buffer_until_newline(
      buffer,
      tag.sequencer,
      MAX_FIELD_LENGTH
  );
}

static char *set_engineer (GmTagDef &tag, char *buffer) {
  buffer = skip_spaces(buffer);
  tag.engineer = static_cast<char *>(malloc(MAX_FIELD_LENGTH));
  return into_buffer_until_newline(
      buffer,
      tag.engineer,
      MAX_FIELD_LENGTH
  );
}

static char *set_date (GmTagDef &tag, char *buffer) {
  buffer = skip_spaces(buffer);
  char cur_char = *buffer++;
  std::string year, month, day;

  // get year
  while ((cur_char >= '0') && (cur_char <= '9')) {
    year.push_back(cur_char);
    cur_char = *buffer++;
  }

  if (cur_char != '-') {
    goto do_parse_date;
  }

  cur_char = *buffer++;

  // get month
  while ((cur_char >= '0') && (cur_char <= '9')) {
    month.push_back(cur_char);
    cur_char = *buffer++;
  }

  if ((cur_char != '-') || (month.length() < 1) ||
      (month.length() > 2))
  {
    goto do_parse_date;
  }

  cur_char = *buffer++;

  // get date
  while ((cur_char >= '0') && (cur_char <= '9')) {
    day.push_back(cur_char);
    cur_char = *buffer++;
  }

do_parse_date:
  if (year == "") {
    year = "0";
  }
  if (month == "") {
    month = "0";
  }
  if (day == "") {
    day = "0";
  }

  tag.date.year = std::stoul(year);
  tag.date.month = static_cast<uint8_t>(std::stoul(month));
  tag.date.day = static_cast<uint8_t>(std::stoul(day));
  return --buffer;
}

static char *set_ripper (GmTagDef &tag, char *buffer) {
  buffer = skip_spaces(buffer);
  tag.ripper = static_cast<char *>(malloc(MAX_FIELD_LENGTH));
  return into_buffer_until_newline(
      buffer,
      tag.ripper,
      MAX_FIELD_LENGTH
  );
}

static char *set_tagger (GmTagDef &tag, char *buffer) {
  buffer = skip_spaces(buffer);
  tag.tagger = static_cast<char *>(malloc(MAX_FIELD_LENGTH));
  return into_buffer_until_newline(
      buffer,
      tag.tagger,
      MAX_FIELD_LENGTH
  );
}

static char *str_to_time (char *buffer, GmTagTimeDef &time) {
  std::string tmp = "";
  std::vector<time_t> test;

  // ↓
  // 00:00:49.000

  char cur_char = *buffer++;
  while ((cur_char >= '0') && (cur_char <= '9')) {
    tmp.push_back(cur_char);
    cur_char = *buffer++;
  }

  if (cur_char == '.') {
    // must be 420.69
    // so the buffer right now must be seconds
    time.seconds = std::stol(tmp);

    // so process just the miliseconds part
    tmp = "";
    cur_char = *(++buffer);
    while ((cur_char >= '0') && (cur_char <= '9')) {
      tmp.push_back(cur_char);
      cur_char = *buffer++;
    }
    time.miliseconds = std::stol(tmp);
    return --buffer;
  } else if (cur_char != ':') {
    // must be 420
    time.seconds = std::stol(tmp);
    return --buffer;
  }

  // save the number for now
  // because I don't know if it's 00:07:00.00
  // or if it's 07:00.00, or if it's 07:00, or if it's 00:07:00
  test.push_back(std::stol(tmp));
  tmp = "";

  cur_char = *buffer++;
  while ((cur_char >= '0') && (cur_char <= '9')) {
    tmp.push_back(cur_char);
    cur_char = *buffer++;
  }
  if (cur_char != ':') {
    // must be 07:00.00 or 07:00
    time.seconds = test.back() * 60;
    time.seconds += std::stol(tmp);

    if (cur_char != '.') {
      // 07:00
      return --buffer;
    }
    // 07:00.00
    tmp = "";
    cur_char = *(++buffer);
    while ((cur_char >= '0') && (cur_char <= '9')) {
      tmp.push_back(cur_char);
      cur_char = *buffer++;
    }
    time.miliseconds = std::stol(tmp);
    return --buffer;
  }
  // 00:07:00 or 00:07:00.00
  test.push_back(std::stol(tmp));
  tmp = "";

  cur_char = *buffer++;
  while ((cur_char >= '0') && (cur_char <= '9')) {
    tmp.push_back(cur_char);
    cur_char = *buffer++;
  }
  // must be 00:07:00.00 or 00:07:00
  // minutes
  time.seconds = test.back() * 60;
  test.pop_back();
  // hours
  time.seconds = test.back() * 60 * 60;
  time.seconds += std::stol(tmp);

  if (cur_char != '.') {
    // 00:07:00
    return --buffer;
  }
  // 00:07:00.00
  tmp = "";
  cur_char = *(++buffer);
  while ((cur_char >= '0') && (cur_char <= '9')) {
    tmp.push_back(cur_char);
    cur_char = *buffer++;
  }
  time.miliseconds = std::stol(tmp);
  return --buffer;
}

static char *set_length (GmTagDef &tag, char *buffer) {
  buffer = skip_spaces(buffer);
  GmTagTimeDef time = GmTagTimeDef{};
  buffer = str_to_time(buffer, time);
  tag.length = time;
  return buffer;
}

static char *set_fade (GmTagDef &tag, char *buffer) {
  buffer = skip_spaces(buffer);
  GmTagTimeDef time = GmTagTimeDef{};
  buffer = str_to_time(buffer, time);
  tag.fade = time;
  return buffer;
}

static char *set_comment (GmTagDef &tag, char *buffer) {
  buffer = skip_spaces(buffer);
  tag.comments = static_cast<char *>(malloc(MAX_FIELD_LENGTH));
  return into_buffer_until_newline(
      buffer,
      tag.comments,
      MAX_FIELD_LENGTH
  );
}

static char *set_copyright (GmTagDef &tag, char *buffer) {
  buffer = skip_spaces(buffer);
  tag.copyright = static_cast<char *>(malloc(MAX_FIELD_LENGTH));
  return into_buffer_until_newline(
      buffer,
      tag.copyright,
      MAX_FIELD_LENGTH
  );
}