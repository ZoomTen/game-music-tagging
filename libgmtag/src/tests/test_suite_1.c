#define MUNIT_ENABLE_ASSERT_ALIASES
#define UNUSED(x) (void)(x)
#include "munit.h"
#include "test_suite_1.h"
#include "libgmtag.h"

#include "./sample_data.h"

void *init_tags (const MunitParameter params[], void *data)
{
  UNUSED(params);
  UNUSED(data);

  tags_from_buffer(tag_init_data);
  return NULL;
}

void deinit_tags (void *data)
{
  UNUSED(data);
  unset_tags();
}

MunitResult
test_subtune_count (const MunitParameter params[], void *data)
{
  UNUSED(params);
  UNUSED(data);
  UNUSED(katakis_3d_test);
  UNUSED(test_spec_derps);

  assert_uint64(get_subtune_count(), ==, 4);
  return MUNIT_OK;
}

MunitResult
test_subtune_order (const MunitParameter params[], void *data)
{
  UNUSED(params);
  UNUSED(data);

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

void check_global_tag (void)
{
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
test_subtune_1 (const MunitParameter params[], void *data)
{
  UNUSED(params);
  UNUSED(data);

  uint64_t i = 1;

  check_global_tag();
  char *title = get_title(i);
  char *composer = get_composer(i);
  char *comment = get_comment(i);

  assert_uint64(get_track_num(i), ==, 1);
  assert_string_equal(title, "Ocean Loader 4");
  assert_string_equal(composer, "Jonathan Dunn");
  assert_string_equal(comment, "Just a short snippet.");
  munit_assert_type(time_t, "lu", get_length(i).seconds, ==, 23);
  munit_assert_type(
      time_t,
      "lu",
      get_length(i).miliseconds,
      ==,
      0
  );
  munit_assert_type(time_t, "lu", get_fade(i).seconds, ==, 5);
  munit_assert_type(
      time_t,
      "lu",
      get_fade(i).miliseconds,
      ==,
      0
  );

  free(comment);
  free(composer);
  free(title);
  return MUNIT_OK;
}

MunitResult
test_subtune_2 (const MunitParameter params[], void *data)
{
  UNUSED(params);
  UNUSED(data);

  uint64_t i = 2;

  check_global_tag();
  char *title = get_title(i);
  char *composer = get_composer(i);
  char *comment = get_comment(i);

  assert_uint64(get_track_num(i), ==, 12);
  assert_string_equal(title, "Super Pitfall - Overworld");
  assert_string_equal(composer, "");  // **NOT NULL!!!**
  assert_string_equal(
      comment,
      "Transcription of the NES version,"
      "\n"
      "with extras from the PC-88 version."
  );
  munit_assert_type(time_t, "lu", get_length(i).seconds, ==, 32);
  munit_assert_type(
      time_t,
      "lu",
      get_length(i).miliseconds,
      ==,
      0
  );
  munit_assert_type(time_t, "lu", get_fade(i).seconds, ==, 10);
  munit_assert_type(
      time_t,
      "lu",
      get_fade(i).miliseconds,
      ==,
      0
  );

  free(comment);
  free(composer);
  free(title);
  return MUNIT_OK;
}

MunitResult
test_subtune_3 (const MunitParameter params[], void *data)
{
  UNUSED(params);
  UNUSED(data);

  uint64_t i = 3;

  check_global_tag();
  char *title = get_title(i);
  char *composer = get_composer(i);
  char *comment = get_comment(i);

  assert_uint64(get_track_num(i), ==, 2);
  assert_string_equal(title, "Pokémon Red & Blue - Intro");
  assert_string_equal(composer, "Junichi Masuda");
  assert_string_equal(comment, "No noise, because I sucked");
  munit_assert_type(time_t, "lu", get_length(i).seconds, ==, 15);
  munit_assert_type(
      time_t,
      "lu",
      get_length(i).miliseconds,
      ==,
      0
  );
  munit_assert_type(time_t, "lu", get_fade(i).seconds, ==, 10);
  munit_assert_type(
      time_t,
      "lu",
      get_fade(i).miliseconds,
      ==,
      0
  );

  free(comment);
  free(composer);
  free(title);
  return MUNIT_OK;
}

MunitResult
test_subtune_4 (const MunitParameter params[], void *data)
{
  UNUSED(params);
  UNUSED(data);

  uint64_t i = 6;

  check_global_tag();
  char *title = get_title(i);
  char *composer = get_composer(i);
  char *comment = get_comment(i);

  assert_uint64(get_track_num(i), ==, 3);
  assert_string_equal(title, "We Are the Crystal Gems");
  assert_string_equal(
      composer,
      "Rebecca Sugar, Aivi & Surasshu"
  );
  assert_string_equal(comment, "");
  munit_assert_type(time_t, "lu", get_length(i).seconds, ==, 54);
  munit_assert_type(
      time_t,
      "lu",
      get_length(i).miliseconds,
      ==,
      0
  );
  munit_assert_type(time_t, "lu", get_fade(i).seconds, ==, 10);
  munit_assert_type(
      time_t,
      "lu",
      get_fade(i).miliseconds,
      ==,
      0
  );

  free(comment);
  free(composer);
  free(title);
  return MUNIT_OK;
}
