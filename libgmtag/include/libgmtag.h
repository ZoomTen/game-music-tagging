/**
 * @file libgmtag.h
 * @brief C interface to libgmtag
 * @version 0.1
 */

#pragma once

#include <stdint.h>

#ifdef __cplusplus
extern "C"
{
#endif
  /** @brief Returned by functions that deal with timing, like
   * get_length(). */
  typedef struct gmtag_timedef
  {
    uint64_t seconds;
    uint64_t miliseconds;
  } GmTagTimeDef;

  /** @brief Returned by functions that deal with date
   * manipulation. */
  typedef struct gmtag_datedef
  {
    uint64_t year;
    uint8_t month;
    uint8_t day;

    uint8_t padding[6];
  } GmTagDateDef;

  /** @brief The main "playlist" for the player to map to actual
   * rip subtunes. */
  typedef struct gmtag_orderdef
  {
    /** @brief Number of elements in the array. */
    uint64_t how_many;

    /** @brief Pointer to a flexible size array of uint64. */
    uint64_t *order;
  } GmTagOrderDef;

  /**
   * @brief Tag handler. This is opaque, and is defined by the implementation.
   */
  typedef void *GmTagObject;

  /**
   * @brief Generate a tags list from the contents of !tags.m3u.
   *
   * The tags list is private to libgmtag, and should be accessed
   * using get_tags_for_subtune(), as that will also handle
   * default tags, should any be defined here.
   *
   * When you're done with tags, you should free its memory by
   * calling unset_tags().
   *
   * @param buff !tags.m3u contents
   */
  GmTagObject tags_from_buffer (const char *buff);

  /**
   * @brief Manually free memory associated with tags.
   *
   * This requires tags to be init'd with tags_from_buffer().
   *
   */
  void unset_tags (GmTagObject handle);

  /**
   * @brief Get the amount of subtunes defined in tags.
   *
   * This requires tags to be init'd with tags_from_buffer().
   *
   * @return int
   */
  uint64_t get_subtune_count (GmTagObject handle);

  /**
   * @brief Get subtune order as a GmTagOrderDef, free manually.
   *
   * This requires tags to be init'd with tags_from_buffer().
   *
   * @return GmTagOrderDef*
   */
  GmTagOrderDef *get_subtune_order (GmTagObject handle);

  /**
   * @brief Get the length of a subtune in miliseconds.
   *
   * @param subtune
   * @return int64_t -1 if subtune doesn't exist
   */
  int64_t get_length_of_subtune (GmTagObject handle, unsigned long subtune);

  /**
   * @brief Get the fade length of a subtune in miliseconds.
   *
   * @param subtune
   * @return int64_t -1 if subtune doesn't exist
   */
  int64_t get_fade_length_of_subtune (GmTagObject handle, unsigned long subtune);

  /**
   * @brief A convenience function to get the total playtime of
   *        a subtune.
   *
   * @param subtune
   * @return int64_t -1 if subtune doesn't exist
   */
  int64_t get_duration_of_subtune (GmTagObject handle, unsigned long subtune);

  // getters and setters for no good reason…
  char *get_album (GmTagObject handle, unsigned long subtune);
  char *get_company (GmTagObject handle, unsigned long subtune);
  char *get_publisher (GmTagObject handle, unsigned long subtune);
  char *get_artist (GmTagObject handle, unsigned long subtune);
  char *get_composer (GmTagObject handle, unsigned long subtune);
  char *get_sequencer (GmTagObject handle, unsigned long subtune);
  char *get_arranger (GmTagObject handle, unsigned long subtune);
  char *get_engineer (GmTagObject handle, unsigned long subtune);
  char *get_ripper (GmTagObject handle, unsigned long subtune);
  char *get_tagger (GmTagObject handle, unsigned long subtune);
  char *get_title (GmTagObject handle, unsigned long subtune);
  char *get_comment (GmTagObject handle, unsigned long subtune);
  char *get_copyright (GmTagObject handle, unsigned long subtune);

  uint64_t get_track_num (GmTagObject handle, unsigned long subtune);
  GmTagDateDef get_date (GmTagObject handle, unsigned long subtune);
  GmTagTimeDef get_length (GmTagObject handle, unsigned long subtune);
  GmTagTimeDef get_fade (GmTagObject handle, unsigned long subtune);

#ifdef __cplusplus
}
#endif
