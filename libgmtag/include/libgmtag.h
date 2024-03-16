#pragma once

#include <stdint.h>
#include <time.h>

#ifdef __cplusplus
extern "C" {
namespace GmTag {
#endif

typedef struct gmtag_timedef {
  time_t seconds;
  time_t miliseconds;
} GmTagTimeDef;

typedef struct gmtag_datedef {
  uint64_t year;
  uint8_t month;
  uint8_t day;

  uint8_t padding[6];
} GmTagDateDef;

typedef struct gmtag_orderdef {
  uint64_t how_many;
  uint64_t *order;  // flexible size array
} GmTagOrderDef;

/**
 * @brief Generate a tags list from the contents of !tags.m3u.
 *
 * The tags list is private to libgmtag, and should be accessed
 * using get_tags_for_subtune(), as that will also handle default
 * tags, should any be defined here.
 *
 * When you're done with tags, you should free its memory by
 * calling unset_tags().
 *
 * @param buff !tags.m3u contents
 */
void tags_from_buffer (const char *buff);

/**
 * @brief Manually free memory associated with tags.
 *
 * This requires tags to be init'd with tags_from_buffer().
 *
 */
void unset_tags (void);

/**
 * @brief Get the amount of subtunes defined in tags.
 *
 * This requires tags to be init'd with tags_from_buffer().
 *
 * @return int
 */
uint64_t get_subtune_count (void);

/**
 * @brief Get subtune order as a GmTagOrderDef, free manually.
 *
 * This requires tags to be init'd with tags_from_buffer().
 *
 * @return GmTagOrderDef*
 */
GmTagOrderDef *get_subtune_order (void);

/**
 * @brief Get the length of a subtune in miliseconds.
 *
 * @param subtune
 * @return int64_t -1 if subtune doesn't exist
 */
int64_t get_length_of_subtune (unsigned long subtune);

/**
 * @brief Get the fade length of a subtune in miliseconds.
 *
 * @param subtune
 * @return int64_t -1 if subtune doesn't exist
 */
int64_t get_fade_length_of_subtune (unsigned long subtune);

/**
 * @brief A convenience function to get the total playtime of
 *        a subtune.
 *
 * @param subtune
 * @return int64_t -1 if subtune doesn't exist
 */
int64_t get_duration_of_subtune (unsigned long subtune);

// getters and setters for no good reason…
char *get_album (unsigned long subtune);
char *get_company (unsigned long subtune);
char *get_publisher (unsigned long subtune);
char *get_artist (unsigned long subtune);
char *get_composer (unsigned long subtune);
char *get_sequencer (unsigned long subtune);
char *get_engineer (unsigned long subtune);
char *get_ripper (unsigned long subtune);
char *get_tagger (unsigned long subtune);
char *get_title (unsigned long subtune);
char *get_comment (unsigned long subtune);
char *get_copyright (unsigned long subtune);

uint64_t get_track_num (unsigned long subtune);
GmTagDateDef get_date (unsigned long subtune);
GmTagTimeDef get_length (unsigned long subtune);
GmTagTimeDef get_fade (unsigned long subtune);

#ifdef __cplusplus
}
}
#endif
