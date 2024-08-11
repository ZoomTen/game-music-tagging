#include "./gmtag.hpp"
#include "./getters.hpp"
#include <string.h>

/**
 * @brief Get the tags for a rip's subtune.
 *
 * If subtune is 0 or a subtune in which tags do not exist for
 * it, it will return the default tags (if any), defined as
 * global tags at the beginning of the file.
 *
 * This requires tags to be init'd with tags_from_buffer().
 *
 * @param subtune
 * @return GmTagDef
 */
GmTagDef
get_tags_for_subtune (GmTagObject handle, unsigned long subtune)
{
  GmTagObjectReal *tags = static_cast<GmTagObjectReal *>(handle);
  if (subtune < 1)
    return (*tags)[0];
  if (tags->count(subtune))
    return (*tags)[subtune];
  return (*tags)[0];
}

uint64_t get_subtune_count (GmTagObject handle)
{
  GmTagObjectReal *tags = static_cast<GmTagObjectReal *>(handle);

  // discount the default tags, so -1
  return tags->size() - 1;
}

int64_t
get_length_of_subtune (GmTagObject handle, unsigned long subtune)
{
  GmTagObjectReal *tags = static_cast<GmTagObjectReal *>(handle);
  if (tags->count(subtune))
  {
    time_t s =
        (((*tags)[subtune].length.seconds * 1000) +
         ((*tags)[subtune].length.miliseconds));
    return s;
  }
  return -1;
}

int64_t get_fade_length_of_subtune (
    GmTagObject handle,
    unsigned long subtune
)
{
  GmTagObjectReal *tags = static_cast<GmTagObjectReal *>(handle);
  if (tags->count(subtune))
  {
    time_t s =
        (((*tags)[subtune].fade.seconds * 1000) +
         ((*tags)[subtune].fade.miliseconds));
    return s;
  }
  return -1;
}

int64_t get_duration_of_subtune (
    GmTagObject handle,
    unsigned long subtune
)
{
  GmTagObjectReal *tags = static_cast<GmTagObjectReal *>(handle);
  if (!tags->count(subtune))
  {
    return -1;
  }
  return get_length_of_subtune(handle, subtune) +
         get_fade_length_of_subtune(handle, subtune);
}

GmTagOrderDef *get_subtune_order (GmTagObject handle)
{
  size_t num_subtunes = get_subtune_count(handle);
  GmTagObjectReal *tags = static_cast<GmTagObjectReal *>(handle);
  GmTagOrderDef *orders =
      static_cast<GmTagOrderDef *>(malloc(sizeof(GmTagOrderDef))
      );

  orders->order = static_cast<uint64_t *>(
      calloc(num_subtunes, sizeof(uint64_t))
  );
  orders->how_many = num_subtunes;

  size_t i = 0;
  for (auto tag : *tags)
  {
    orders->order[i++] = tag.first;
  }

  return orders;
}

char *get_album (GmTagObject handle, unsigned long subtune)
{
  GmTagDef tag = get_tags_for_subtune(handle, subtune);
  char *i = strdup(tag.album.c_str());
  return i;
}

char *get_company (GmTagObject handle, unsigned long subtune)
{
  GmTagDef tag = get_tags_for_subtune(handle, subtune);
  char *i = strdup(tag.company.c_str());
  return i;
}

char *get_publisher (GmTagObject handle, unsigned long subtune)
{
  GmTagDef tag = get_tags_for_subtune(handle, subtune);
  char *i = strdup(tag.publisher.c_str());
  return i;
}

char *get_artist (GmTagObject handle, unsigned long subtune)
{
  GmTagDef tag = get_tags_for_subtune(handle, subtune);
  char *i = strdup(tag.artist.c_str());
  return i;
}

char *get_composer (GmTagObject handle, unsigned long subtune)
{
  GmTagDef tag = get_tags_for_subtune(handle, subtune);
  char *i = strdup(tag.composer.c_str());
  return i;
}

char *get_sequencer (GmTagObject handle, unsigned long subtune)
{
  GmTagDef tag = get_tags_for_subtune(handle, subtune);
  char *i = strdup(tag.sequencer.c_str());
  return i;
}

char *get_engineer (GmTagObject handle, unsigned long subtune)
{
  GmTagDef tag = get_tags_for_subtune(handle, subtune);
  char *i = strdup(tag.engineer.c_str());
  return i;
}

char *get_ripper (GmTagObject handle, unsigned long subtune)
{
  GmTagDef tag = get_tags_for_subtune(handle, subtune);
  char *i = strdup(tag.ripper.c_str());
  return i;
}

char *get_tagger (GmTagObject handle, unsigned long subtune)
{
  GmTagDef tag = get_tags_for_subtune(handle, subtune);
  char *i = strdup(tag.tagger.c_str());
  return i;
}

char *get_title (GmTagObject handle, unsigned long subtune)
{
  GmTagDef tag = get_tags_for_subtune(handle, subtune);
  char *i = strdup(tag.title.c_str());
  return i;
}

char *get_comment (GmTagObject handle, unsigned long subtune)
{
  GmTagDef tag = get_tags_for_subtune(handle, subtune);
  char *i = strdup(tag.comments.c_str());
  return i;
}

char *get_copyright (GmTagObject handle, unsigned long subtune)
{
  GmTagDef tag = get_tags_for_subtune(handle, subtune);
  char *i = strdup(tag.copyright.c_str());
  return i;
}

GmTagDateDef get_date (GmTagObject handle, unsigned long subtune)
{
  GmTagDef tag = get_tags_for_subtune(handle, subtune);
  return tag.date;
}

GmTagTimeDef
get_length (GmTagObject handle, unsigned long subtune)
{
  GmTagDef tag = get_tags_for_subtune(handle, subtune);
  return tag.length;
}

GmTagTimeDef get_fade (GmTagObject handle, unsigned long subtune)
{
  GmTagDef tag = get_tags_for_subtune(handle, subtune);
  return tag.fade;
}

uint64_t
get_track_num (GmTagObject handle, unsigned long subtune)
{
  GmTagDef tag = get_tags_for_subtune(handle, subtune);
  return tag.track;
}
