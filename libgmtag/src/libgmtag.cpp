#include "libgmtag.h"

#include <algorithm>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <vector>
#include <map>
#include <string>

typedef char *(*BufOpFunc)(GmTagDef &, char *);

// map a track number to a GmTagDef
static std::map<uint64_t, GmTagDef> tags;

// each tag has a handler function
static std::map<std::string, BufOpFunc> tag_handlers;
static bool tag_handlers_init = false;

// "global" tags
static GmTagDef default_tags = {};
static uint64_t track_num = 1;

// every field has a maximum length of:
#define MAX_FIELD_LENGTH 256

// utilities
static char *skip_spaces (char *);
static char *skip_current_line (char *);
static char *word_into_buffer (char *, char *, size_t);
static char *into_buffer_until_newline (char *, char *, size_t);
static inline bool is_number (char i);

// handlers
static char *set_track (GmTagDef &, char *);
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

// API
void tags_from_buffer (char *buff) {
  char *buff_pointer = buff;

  if (!tag_handlers_init) {
    tag_handlers["track"] = set_track;
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

  GmTagDef current_tag = GmTagDef{};

  char *keyword = static_cast<char *>(malloc(MAX_FIELD_LENGTH));

  char current_sigil;
  char last_sigil;

  // while not EOF
  while (*buff_pointer != '\0') {
    // I assume every iteration starts at the
    // beginning of the line
    buff_pointer = skip_spaces(buff_pointer);
    std::string word;

    switch (*buff_pointer) {
      case '#':
        // this is an m3u comment. does it contain tags or not?
        buff_pointer++;
        buff_pointer = skip_spaces(buff_pointer);

        switch (current_sigil = *buff_pointer++) {
          case '@':  // global tag
          case '%':  // local tag
            if ((current_sigil == '%') && (last_sigil == '@')) {
              // assume this is where the group of
              // "global" tags end and where the
              // "local" tags start, so save the "global"
              // tags up to this point.
              default_tags = current_tag;
            }

            buff_pointer = word_into_buffer(
                buff_pointer,
                keyword,
                MAX_FIELD_LENGTH - 1
            );

            word = std::string(keyword);

            // normalize case: https://stackoverflow.com/a/313990
            std::transform(
                word.begin(),
                word.end(),
                word.begin(),
                [] (unsigned char c) { return std::tolower(c); }
            );

            if (tag_handlers.count(word)) {
              buff_pointer =
                  tag_handlers[word](current_tag, buff_pointer);
            }
            break;
          default:
            break;
        }

        last_sigil = current_sigil;
        buff_pointer = skip_current_line(buff_pointer);
        break;
      case '\n':
      case '\r':
        buff_pointer = skip_current_line(buff_pointer);
        break;
      default:
        // this must be an m3u entry, attempt to parse it
        bool next_is_subtune_number = false;
        std::string subtune_number_str = "";

        while ((*buff_pointer != '\0') &&
               (*buff_pointer != '\n') &&
               (*buff_pointer != '\r'))
        {
          if (*buff_pointer == '?') {
            // in case we have titles like:
            // "Where the HELL is Carmen Sandiego???.nsf?4"
            char next_is = *(buff_pointer + 1);
            next_is_subtune_number = is_number(next_is);
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
            if (current_tag.track)
              prev_tag.track = current_tag.track;

            if (prev_tag.track == 0)
              prev_tag.track = track_num++;

            tags[subtune_num] = prev_tag;
          } else {
            // otherwise, just add it
            if (current_tag.track == 0)
              current_tag.track = track_num++;

            tags[subtune_num] = current_tag;
          }

          // create new tag
          current_tag = default_tags;
          buff_pointer = skip_current_line(buff_pointer);
        }
        break;
    }
  }

  free(keyword);
}

GmTagDef get_tags_for_subtune (unsigned long subtune) {
  if (tags.count(subtune))
    return tags[subtune];
  return default_tags;
}

void unset_tags () {
  {  // unset default tags
    if (default_tags.album) {
      free(default_tags.album);
    }
    if (default_tags.company) {
      free(default_tags.company);
    }
    if (default_tags.publisher) {
      free(default_tags.publisher);
    }
    if (default_tags.artist) {
      free(default_tags.artist);
    }
    if (default_tags.composer) {
      free(default_tags.composer);
    }
    if (default_tags.sequencer) {
      free(default_tags.sequencer);
    }
    if (default_tags.engineer) {
      free(default_tags.engineer);
    }
    if (default_tags.ripper) {
      free(default_tags.ripper);
    }
    if (default_tags.tagger) {
      free(default_tags.tagger);
    }
    if (default_tags.title) {
      free(default_tags.title);
    }
    if (default_tags.comments) {
      free(default_tags.comments);
    }
    if (default_tags.copyright) {
      free(default_tags.copyright);
    }
  }
  {  // then unset unique tags
    for (auto tag : tags) {
      if (tag.second.album != default_tags.album) {
        free(tag.second.album);
      }
      if (tag.second.company != default_tags.company) {
        free(tag.second.company);
      }
      if (tag.second.publisher != default_tags.publisher) {
        free(tag.second.publisher);
      }
      if (tag.second.artist != default_tags.artist) {
        free(tag.second.artist);
      }
      if (tag.second.composer != default_tags.composer) {
        free(tag.second.composer);
      }
      if (tag.second.sequencer != default_tags.sequencer) {
        free(tag.second.sequencer);
      }
      if (tag.second.engineer != default_tags.engineer) {
        free(tag.second.engineer);
      }
      if (tag.second.ripper != default_tags.ripper) {
        free(tag.second.ripper);
      }
      if (tag.second.tagger != default_tags.tagger) {
        free(tag.second.tagger);
      }
      if (tag.second.title != default_tags.title) {
        free(tag.second.title);
      }
      if (tag.second.comments != default_tags.comments) {
        free(tag.second.comments);
      }
      if (tag.second.copyright != default_tags.copyright) {
        free(tag.second.copyright);
      }
    }
  }
}

uint64_t get_subtune_count () { return tags.size(); }

int64_t get_length_of_subtune (unsigned long subtune) {
  if (tags.count(subtune)) {
    time_t s =
        ((tags[subtune].length.seconds * 1000) +
         (tags[subtune].length.miliseconds));
    return s;
  }
  return -1;
}

int64_t get_fade_length_of_subtune (unsigned long subtune) {
  if (tags.count(subtune)) {
    time_t s =
        ((tags[subtune].fade.seconds * 1000) +
         (tags[subtune].fade.miliseconds));
    return s;
  }
  return -1;
}

int64_t get_duration_of_subtune (unsigned long subtune) {
  if (!tags.count(subtune)) {
    return -1;
  }
  return get_length_of_subtune(subtune) +
         get_fade_length_of_subtune(subtune);
}

GmTagOrderDef *get_subtune_order () {
  GmTagOrderDef *orders =
      static_cast<GmTagOrderDef *>(malloc(sizeof(GmTagOrderDef))
      );

  orders->order = static_cast<uint64_t *>(
      calloc(tags.size(), sizeof(uint64_t))
  );
  orders->how_many = tags.size();

  size_t i = 0;
  for (auto tag : tags) {
    orders->order[i++] = tag.first;
  }

  return orders;
}

// Util defines

inline bool is_number (char i) {
  return ((i >= '0') && (i <= '9'));
}

// skip spaces
char *skip_spaces (char *i) {
  while ((*i == ' ') || (*i == '\t')) {
    i++;
  }
  return i;
}

char *skip_current_line (char *i) {
  char current_char = *i;

  while ((current_char != '\r') && (current_char != '\n') && (current_char != '\0')) {
    i++;
    current_char = *i;
  }
  switch (current_char) {
    case '\0':
      // if EOF, just return the end addr
      return i;
    case '\r':
      // dos/windows line ending
      i++; // always assume \n, classic mac discounted
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
  while (is_number(cur_char)) {
    year.push_back(cur_char);
    cur_char = *buffer++;
  }

  if (cur_char != '-') {
    goto do_parse_date;
  }

  cur_char = *buffer++;

  // get month
  while (is_number(cur_char)) {
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
  while (is_number(cur_char)) {
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
  // in case this was called with uninit'd &time
  time.seconds = 0;
  time.miliseconds = 0;

  std::string tmp = "";
  std::vector<time_t> tmp_stack;

  char cur_char = *buffer++;
  while (is_number(cur_char)) {
    tmp.push_back(cur_char);
    cur_char = *buffer++;
  }

  if (cur_char == '.') {
    // must be 420.690
    // so the buffer right now must be seconds
    time.seconds = std::stol(tmp);

    // then process just the miliseconds part
    tmp = "";
    cur_char = *(++buffer);
    while (is_number(cur_char)) {
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
  // because I don't know if it's 00:07:00.690
  // or if it's 07:00.690 or what…
  tmp_stack.push_back(std::stol(tmp));
  tmp = "";

  cur_char = *buffer++;
  while (is_number(cur_char)) {
    tmp.push_back(cur_char);
    cur_char = *buffer++;
  }

  if (cur_char != ':') {
    // must be 07:00.690 or 07:00
    time.seconds = tmp_stack.back() * 60;
    time.seconds += std::stol(tmp);

    if (cur_char != '.') {  // 07:00
      return --buffer;
    }

    // 07:00.690
    tmp = "";
    cur_char = *(++buffer);
    while (is_number(cur_char)) {
      tmp.push_back(cur_char);
      cur_char = *buffer++;
    }
    time.miliseconds = std::stol(tmp);
    return --buffer;
  }

  // this leaves us with 00:07:00 or 00:07:00.690
  tmp_stack.push_back(std::stol(tmp));
  tmp = "";

  cur_char = *buffer++;
  while (is_number(cur_char)) {
    tmp.push_back(cur_char);
    cur_char = *buffer++;
  }

  // minutes
  time.seconds = tmp_stack.back() * 60;
  tmp_stack.pop_back();

  // hours
  time.seconds = tmp_stack.back() * 60 * 60;
  time.seconds += std::stol(tmp);

  if (cur_char != '.') {
    // 00:07:00
    return --buffer;
  }

  // 00:07:00.690
  tmp = "";
  cur_char = *(++buffer);
  while (is_number(cur_char)) {
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
  if (tag.comments) {
    // append to existing
    size_t i = 0;
    char *cmttag = tag.comments;
    // find end of comment field
    while (*cmttag++ != '\0') {
      i++;
    }
    cmttag--;
    *cmttag++ = '\n';
    return into_buffer_until_newline(
        buffer,
        cmttag,
        MAX_FIELD_LENGTH - i
    );
  }
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

static char *set_track (GmTagDef &tag, char *buffer) {
  buffer = skip_spaces(buffer);
  std::string tmp;
  char cur_char = *buffer++;
  while (is_number(cur_char)) {
    tmp.push_back(cur_char);
    cur_char = *buffer++;
  }
  if (tmp.length() > 0) {
    tag.track = std::stoul(tmp);
  }
  return --buffer;
}