#define MUNIT_ENABLE_ASSERT_ALIASES
#include "munit.h"
#include "test_suite_1.h"
#include "libgmtag.h"

#include "./sample_data.h"

void *init_tags (const MunitParameter params[], void *data) {
  tags_from_buffer(tag_init_data);
  return NULL;
}

void deinit_tags (void *data) { unset_tags(); }

MunitResult
test_subtune_count (const MunitParameter params[], void *data) {
  assert_uint64(get_subtune_count(), ==, 4);
  return MUNIT_OK;
}

MunitResult
test_subtune_order (const MunitParameter params[], void *data) {
  GmTagOrderDef *order = get_subtune_order();
  assert_uint64(order->how_many, ==, 4);
  assert_uint64(order->order[0], ==, 1);
  assert_uint64(order->order[1], ==, 2);
  assert_uint64(order->order[2], ==, 3);
  assert_uint64(order->order[3], ==, 6);
  free(order->order);
  free(order);
  return MUNIT_OK;
}

void check_global_tag (void) {
  char *album = get_album(0);
  char *company = get_company(0);
  char *publisher = get_publisher(0);
  char *artist = get_artist(0);
  char *ripper = get_ripper(0);
  char *tagger = get_tagger(0);
  char *sequencer = get_sequencer(0);
  char *engineer = get_engineer(0);

  assert_string_equal(album, "Zumi's GB Music Engine v1");
  assert_string_equal(company, "Zumi");
  assert_string_equal(publisher, "none");
  assert_string_equal(artist, "Unknown");
  assert_uint64(get_date(0).year, ==, 2015);
  assert_uint8(get_date(0).month, ==, 10);
  assert_uint8(get_date(0).day, ==, 9);
  assert_string_equal(ripper, "Zumi");
  assert_string_equal(tagger, "Zumi");
  assert_string_equal(sequencer, "Zumi");
  assert_string_equal(engineer, "Zumi");

  free(album);
  free(company);
  free(publisher);
  free(artist);
  free(ripper);
  free(tagger);
  free(sequencer);
  free(engineer);
}

MunitResult
test_subtune_1 (const MunitParameter params[], void *data) {
  check_global_tag();
  char *title = get_title(1);
  char *composer = get_composer(1);
  char *comment = get_comment(1);

  assert_uint64(get_track_num(1), ==, 1);
  assert_string_equal(title, "Ocean Loader 4");
  assert_string_equal(composer, "Jonathan Dunn");
  assert_string_equal(comment, "Just a short snippet.");
  munit_assert_type(time_t, "lu", get_length(1).seconds, ==, 23);
  munit_assert_type(
      time_t,
      "lu",
      get_length(1).miliseconds,
      ==,
      0
  );
  munit_assert_type(time_t, "lu", get_fade(1).seconds, ==, 5);
  munit_assert_type(time_t, "lu", get_fade(1).miliseconds, ==, 0);

  free(comment);
  free(composer);
  free(title);
  return MUNIT_OK;
}

MunitResult
test_subtune_2 (const MunitParameter params[], void *data) {
  return MUNIT_SKIP;
  #if 0
  GmTagDef tags = get_tags_for_subtune(2);
  check_global_tag(tags);
  assert_uint64(tags.track, ==, 12);
  assert_string_equal(tags.title, "Super Pitfall - Overworld");
  assert_ptr_equal(tags.composer, NULL);
  assert_string_equal(
      tags.comments,
      "Transcription of the NES version,\nwith extras from the "
      "PC-88 version."
  );
  munit_assert_type(time_t, "lu", tags.length.seconds, ==, 32);
  munit_assert_type(
      time_t,
      "lu",
      tags.length.miliseconds,
      ==,
      0
  );
  munit_assert_type(time_t, "lu", tags.fade.seconds, ==, 10);
  munit_assert_type(time_t, "lu", tags.fade.miliseconds, ==, 0);
  return MUNIT_OK;
  #endif
}

MunitResult
test_subtune_3 (const MunitParameter params[], void *data) {
return MUNIT_SKIP;
#if 0
  GmTagDef tags = get_tags_for_subtune(3);
  check_global_tag(tags);
  assert_uint64(tags.track, ==, 2);
  assert_string_equal(tags.title, "Pokémon Red & Blue - Intro");
  assert_string_equal(tags.composer, "Junichi Masuda");
  assert_string_equal(
      tags.comments,
      "No noise, because I sucked"
  );
  munit_assert_type(time_t, "lu", tags.length.seconds, ==, 15);
  munit_assert_type(
      time_t,
      "lu",
      tags.length.miliseconds,
      ==,
      0
  );
  munit_assert_type(time_t, "lu", tags.fade.seconds, ==, 10);
  munit_assert_type(time_t, "lu", tags.fade.miliseconds, ==, 0);
  return MUNIT_OK;
  #endif
}

MunitResult
test_subtune_4 (const MunitParameter params[], void *data) {
return MUNIT_SKIP;
#if 0
  GmTagDef tags = get_tags_for_subtune(6);
  check_global_tag(tags);
  assert_uint64(tags.track, ==, 3);
  assert_string_equal(tags.title, "We Are the Crystal Gems");
  assert_string_equal(
      tags.composer,
      "Rebecca Sugar, Aivi & Surasshu"
  );
  assert_ptr_equal(tags.comments, NULL);
  munit_assert_type(time_t, "lu", tags.length.seconds, ==, 54);
  munit_assert_type(
      time_t,
      "lu",
      tags.length.miliseconds,
      ==,
      0
  );
  munit_assert_type(time_t, "lu", tags.fade.seconds, ==, 10);
  munit_assert_type(time_t, "lu", tags.fade.miliseconds, ==, 0);
  return MUNIT_OK;
  #endif
}
