#pragma once

#include <stdint.h>
#include <time.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct gmtag_timedef {
  time_t seconds;
  time_t miliseconds;
} GmTagTimeDef;

typedef struct gmtag_datedef {
  uint64_t year;
  uint8_t month;
  uint8_t day;
} GmTagDateDef;

typedef struct gmtag_def {
  char *album;
  char *company;
  char *publisher;
  char *artist;
  char *composer;
  char *sequencer;
  char *engineer;
  GmTagDateDef date;
  char *ripper;
  char *tagger;
  char *source;
  char *title;
  char *comments;
  char *copyright;
  GmTagTimeDef length;
  GmTagTimeDef fade;
} GmTagDef;

void tags_from_buffer (char *buff);
GmTagDef get_tags_for_subtune (int subtune);

#ifdef __cplusplus
}
#endif