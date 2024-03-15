#include <algorithm>
#include <cstdint>
#include <cstdlib>
#include <map>
#include <string>
#include "libgmtag.h"
#include "./private/gmtag.hpp"
#include "./private/utils.hpp"
#include "./private/setters.hpp"

typedef const char *(*BufOpFunc)(GmTagDef &, const char *);

// map a track number to a GmTagDef
std::map<uint64_t, GmTagDef> tags;

// "global" tags
GmTagDef default_tags = {};

// API
void tags_from_buffer (const char *buff) {
  static bool tag_handlers_init = false;
  static std::map<std::string, BufOpFunc> tag_handlers;

  const char *buff_pointer = buff;

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

  tags.clear();
  GmTagDef current_tag = GmTagDef{};
  uint64_t track_num = 1;

  char *keyword = static_cast<char *>(malloc(MAX_FIELD_LENGTH));

  char current_sigil = 0;
  char last_sigil = 0;

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

            if (!current_tag.album.empty())
              prev_tag.album = current_tag.album;
            if (!current_tag.company.empty())
              prev_tag.company = current_tag.company;
            if (!current_tag.publisher.empty())
              prev_tag.publisher = current_tag.publisher;
            if (!current_tag.artist.empty())
              prev_tag.artist = current_tag.artist;
            if (!current_tag.composer.empty())
              prev_tag.composer = current_tag.composer;
            if (!current_tag.sequencer.empty())
              prev_tag.sequencer = current_tag.sequencer;
            if (!current_tag.engineer.empty())
              prev_tag.engineer = current_tag.engineer;
            // if (current_tag.date) prev_tag.date =
            // current_tag.date;
            if (!current_tag.ripper.empty())
              prev_tag.ripper = current_tag.ripper;
            if (!current_tag.tagger.empty())
              prev_tag.tagger = current_tag.tagger;
            if (!current_tag.title.empty())
              prev_tag.title = current_tag.title;
            if (!current_tag.comments.empty())
              prev_tag.comments = current_tag.comments;
            if (!current_tag.copyright.empty())
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

void unset_tags () { tags.clear(); }
