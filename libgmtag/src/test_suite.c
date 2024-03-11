#include "munit.h"
#include "test_suite.h"
#include "libgmtag.h"

static const char *tag_init_data =
    "## This !tags.m3u is formatted weirdly\n"
    "## in order to test how sloppy the library\n"
    "## can take it\n"
    "\n"
    "#@ALBUM Zumi's GB Music Engine v1\n"
    "#@company Zumi\n"
    "# @Publisher none\n"
    "# @aRTIST Unknown\n"
    "#@date  2015-10-09\n"
    "#@ripper Zumi\n"
    "#@tagger Zumi\n"
    "# @sequencer Zumi\n"
    "# @engineer Zumi\n"
    "#@fade 00:00:10.000\n"
    "\n"
    "# %title Ocean Loader 4\n"
    "# %composer Jonathan Dunn\n"
    "# %comment Just a short snippet.\n"
    "# %length 0:23\n"
    "# %fade 5\n"
    "\n"
    "gbstest.gbs?1\n"
    "\n"
    "#   %track 12\n"
    "# %title Super Pitfall - Overworld\n"
    "# %length 32.000\n"
    "	#	%comment Transcription of the NES version,\n"
    "	#%comment with extras from the PC-88 version.\n"
    "gbstest.gbs?2\n"
    "\n"
    "# %title Pokémon Red & Blue - Intro\n"
    "# %length 00:15.000\n"
    "#%composer Junichi Masuda\n"
    "#	%comment No noise, because I sucked\n"
    "\n"
    "\n"
    "gbstest.gbs?3\n"
    "\n"
    "# %title We Are the Crystal Gems\n"
    "# %length 0:00:54.000\n"
    "# %composer Rebecca Sugar, Aivi & Surasshu\n"
    "gbstest.gbs?6\n";

void *init_tags (const MunitParameter params[], void *data) {
  tags_from_buffer(tag_init_data);
  return NULL;
}

void deinit_tags (void *data) {
  unset_tags();
}

MunitResult
test_subtune_count (const MunitParameter params[], void *data) {
  munit_assert_uint64(get_subtune_count(), ==, 4);
  return MUNIT_OK;
}

MunitResult
test_subtune_order (const MunitParameter params[], void *data) {
  GmTagOrderDef *order = get_subtune_order();
  munit_assert_uint64(order->how_many, ==, 4);
  munit_assert_uint64(order->order[0], ==, 1);
  munit_assert_uint64(order->order[1], ==, 2);
  munit_assert_uint64(order->order[2], ==, 3);
  munit_assert_uint64(order->order[3], ==, 6);
  free(order->order);
  free(order);
  return MUNIT_OK;
}

void
check_global_tag (GmTagDef tags) {
  munit_assert_string_equal(tags.album, "Zumi's GB Music Engine v1");
  munit_assert_string_equal(tags.company, "Zumi");
  munit_assert_string_equal(tags.publisher, "none");
  munit_assert_string_equal(tags.artist, "Unknown");
  munit_assert_uint64(tags.date.year, ==, 2015);
  munit_assert_uint8(tags.date.month, ==, 10);
  munit_assert_uint8(tags.date.day, ==, 9);
  munit_assert_string_equal(tags.ripper, "Zumi");
  munit_assert_string_equal(tags.tagger, "Zumi");
  munit_assert_string_equal(tags.sequencer, "Zumi");
  munit_assert_string_equal(tags.engineer, "Zumi");
}

MunitResult
test_subtune_1 (const MunitParameter params[], void *data) {
  GmTagDef tags = get_tags_for_subtune(1);
  check_global_tag(tags);
  munit_assert_uint64(tags.track, ==, 1);
  munit_assert_string_equal(tags.title, "Ocean Loader 4");
  munit_assert_string_equal(tags.composer, "Jonathan Dunn");
  munit_assert_string_equal(tags.comments, "Just a short snippet.");
  munit_assert_type(time_t, "lu", tags.length.seconds, ==, 23);
  munit_assert_type(time_t, "lu", tags.length.miliseconds, ==, 0);
  munit_assert_type(time_t, "lu", tags.fade.seconds, ==, 5);
  munit_assert_type(time_t, "lu", tags.fade.miliseconds, ==, 0);
  return MUNIT_OK;
}

MunitResult
test_subtune_2 (const MunitParameter params[], void *data) {
  GmTagDef tags = get_tags_for_subtune(2);
  check_global_tag(tags);
  munit_assert_uint64(tags.track, ==, 12);
  munit_assert_string_equal(tags.title, "Super Pitfall - Overworld");
  munit_assert_ptr_equal(tags.composer, NULL);
  munit_assert_string_equal(tags.comments, "Transcription of the NES version,\nwith extras from the PC-88 version.");
  munit_assert_type(time_t, "lu", tags.length.seconds, ==, 32);
  munit_assert_type(time_t, "lu", tags.length.miliseconds, ==, 0);
  munit_assert_type(time_t, "lu", tags.fade.seconds, ==, 10);
  munit_assert_type(time_t, "lu", tags.fade.miliseconds, ==, 0);
  return MUNIT_OK;
}

MunitResult
test_subtune_3 (const MunitParameter params[], void *data) {
  GmTagDef tags = get_tags_for_subtune(3);
  check_global_tag(tags);
  munit_assert_uint64(tags.track, ==, 2);
  munit_assert_string_equal(tags.title, "Pokémon Red & Blue - Intro");
  munit_assert_string_equal(tags.composer, "Junichi Masuda");
  munit_assert_string_equal(tags.comments, "No noise, because I sucked");
  munit_assert_type(time_t, "lu", tags.length.seconds, ==, 15);
  munit_assert_type(time_t, "lu", tags.length.miliseconds, ==, 0);
  munit_assert_type(time_t, "lu", tags.fade.seconds, ==, 10);
  munit_assert_type(time_t, "lu", tags.fade.miliseconds, ==, 0);
  return MUNIT_OK;
}

MunitResult
test_subtune_4 (const MunitParameter params[], void *data) {
  GmTagDef tags = get_tags_for_subtune(6);
  check_global_tag(tags);
  munit_assert_uint64(tags.track, ==, 3);
  munit_assert_string_equal(tags.title, "We Are the Crystal Gems");
  munit_assert_string_equal(tags.composer, "Rebecca Sugar, Aivi & Surasshu");
  munit_assert_ptr_equal(tags.comments, NULL);
  munit_assert_type(time_t, "lu", tags.length.seconds, ==, 54);
  munit_assert_type(time_t, "lu", tags.length.miliseconds, ==, 0);
  munit_assert_type(time_t, "lu", tags.fade.seconds, ==, 10);
  munit_assert_type(time_t, "lu", tags.fade.miliseconds, ==, 0);
  return MUNIT_OK;
}