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

void check_global_tag (GmTagDef tags) {
  assert_string_equal(tags.album, "Zumi's GB Music Engine v1");
  assert_string_equal(tags.company, "Zumi");
  assert_string_equal(tags.publisher, "none");
  assert_string_equal(tags.artist, "Unknown");
  assert_uint64(tags.date.year, ==, 2015);
  assert_uint8(tags.date.month, ==, 10);
  assert_uint8(tags.date.day, ==, 9);
  assert_string_equal(tags.ripper, "Zumi");
  assert_string_equal(tags.tagger, "Zumi");
  assert_string_equal(tags.sequencer, "Zumi");
  assert_string_equal(tags.engineer, "Zumi");
}

MunitResult
test_subtune_1 (const MunitParameter params[], void *data) {
  GmTagDef tags = get_tags_for_subtune(1);
  check_global_tag(tags);
  assert_uint64(tags.track, ==, 1);
  assert_string_equal(tags.title, "Ocean Loader 4");
  assert_string_equal(tags.composer, "Jonathan Dunn");
  assert_string_equal(tags.comments, "Just a short snippet.");
  munit_assert_type(time_t, "lu", tags.length.seconds, ==, 23);
  munit_assert_type(
      time_t,
      "lu",
      tags.length.miliseconds,
      ==,
      0
  );
  munit_assert_type(time_t, "lu", tags.fade.seconds, ==, 5);
  munit_assert_type(time_t, "lu", tags.fade.miliseconds, ==, 0);
  return MUNIT_OK;
}

MunitResult
test_subtune_2 (const MunitParameter params[], void *data) {
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
}

MunitResult
test_subtune_3 (const MunitParameter params[], void *data) {
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
}

MunitResult
test_subtune_4 (const MunitParameter params[], void *data) {
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
}
