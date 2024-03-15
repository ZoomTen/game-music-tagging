#define MUNIT_ENABLE_ASSERT_ALIASES
#include "munit.h"
#include "test_suite_2.h"
#include "libgmtag.h"

#include "./sample_data.h"

void *init_tags_2 (const MunitParameter params[], void *data) {
  tags_from_buffer(katakis_3d_test);
  return NULL;
}

void deinit_tags_2 (void *data) { unset_tags(); }

#define TIME_TO_MS(h, m, s, ms) \
  (h * 60 * 60 * 1000) + (m * 60 * 1000) + (s * 1000) + (ms)

MunitResult
test_migrated_tags (const MunitParameter params[], void *data) {
  return MUNIT_SKIP;
#if 0
  GmTagDef tags;
  for (size_t i = 1; i <= 18; i++) {
    tags = get_tags_for_subtune(i);
    assert_uint64(tags.track, ==, i);
    // check global tags
    assert_string_equal(tags.album, "Katakis 3D");
    assert_string_equal(tags.company, "Similis");
    assert_string_equal(tags.ripper, "zlago");
    assert_string_equal(tags.tagger, "Zumi, zlago");
    assert_uint64(tags.date.year, ==, 2001);
    // check artist
    switch (i) {
      case 1:
        assert_string_equal(
            tags.artist,
            "Chris Huelsbeck, Tufan Uysal"
        );
        break;
      default:
        assert_string_equal(tags.artist, "Tufan Uysal");
        break;
    }
    // check other metadata
    switch (i) {
      case 1:
        assert_string_equal(tags.title, "Katakis (Remix)");
        assert_uint64(
            get_duration_of_subtune(i),
            ==,
            TIME_TO_MS(0, 3, 17, 0)
        );
        break;
      case 2:
        assert_string_equal(tags.title, "Loud 'n Proud");
        assert_uint64(
            get_duration_of_subtune(i),
            ==,
            TIME_TO_MS(0, 0, 53, 0)
        );
        break;
      case 3:
        assert_string_equal(tags.title, "Flight to Hell");
        assert_uint64(
            get_duration_of_subtune(i),
            ==,
            TIME_TO_MS(0, 1, 18, 0)
        );
        break;
      case 4:
        assert_string_equal(tags.title, "The Big Thing");
        assert_uint64(
            get_duration_of_subtune(i),
            ==,
            TIME_TO_MS(0, 0, 49, 0)
        );
        break;
      case 5:
        assert_string_equal(tags.title, "Electrical Motions");
        assert_uint64(
            get_duration_of_subtune(i),
            ==,
            TIME_TO_MS(0, 1, 31, 0)
        );
        break;
      case 6:
        assert_string_equal(tags.title, "Protected Beat");
        assert_uint64(
            get_duration_of_subtune(i),
            ==,
            TIME_TO_MS(0, 0, 24, 0)
        );
        break;
      case 7:
        assert_string_equal(tags.title, "Secret Cycles");
        assert_uint64(
            get_duration_of_subtune(i),
            ==,
            TIME_TO_MS(0, 1, 52, 0)
        );
        assert_uint64(
            get_fade_length_of_subtune(i),
            ==,
            TIME_TO_MS(0, 0, 10, 0)
        );
        assert_uint64(
            get_duration_of_subtune(i),
            ==,
            TIME_TO_MS(0, 2, 3, 0)
        );
        break;
      case 8:
        assert_string_equal(tags.title, "Radioactive Attack");
        assert_uint64(
            get_duration_of_subtune(i),
            ==,
            TIME_TO_MS(0, 0, 42, 0)
        );
        break;
#if 0
      case 2:
        assert_string_equal(tags.title, "Loud 'n Proud");
        assert_uint64(
            get_duration_of_subtune(i),
            ==,
            TIME_TO_MS(0, 53, 0, 0)
        );
        break;
      case 2:
        assert_string_equal(tags.title, "Loud 'n Proud");
        assert_uint64(
            get_duration_of_subtune(i),
            ==,
            TIME_TO_MS(0, 53, 0, 0)
        );
        break;
      case 2:
        assert_string_equal(tags.title, "Loud 'n Proud");
        assert_uint64(
            get_duration_of_subtune(i),
            ==,
            TIME_TO_MS(0, 53, 0, 0)
        );
        break;
      case 2:
        assert_string_equal(tags.title, "Loud 'n Proud");
        assert_uint64(
            get_duration_of_subtune(i),
            ==,
            TIME_TO_MS(0, 53, 0, 0)
        );
        break;
      case 2:
        assert_string_equal(tags.title, "Loud 'n Proud");
        assert_uint64(
            get_duration_of_subtune(i),
            ==,
            TIME_TO_MS(0, 53, 0, 0)
        );
        break;
      case 2:
        assert_string_equal(tags.title, "Loud 'n Proud");
        assert_uint64(
            get_duration_of_subtune(i),
            ==,
            TIME_TO_MS(0, 53, 0, 0)
        );
        break;
      case 2:
        assert_string_equal(tags.title, "Loud 'n Proud");
        assert_uint64(
            get_duration_of_subtune(i),
            ==,
            TIME_TO_MS(0, 53, 0, 0)
        );
        break;
      case 2:
        assert_string_equal(tags.title, "Loud 'n Proud");
        assert_uint64(
            get_duration_of_subtune(i),
            ==,
            TIME_TO_MS(0, 53, 0, 0)
        );
        break;
      case 2:
        assert_string_equal(tags.title, "Loud 'n Proud");
        assert_uint64(
            get_duration_of_subtune(i),
            ==,
            TIME_TO_MS(0, 53, 0, 0)
        );
        break;
      case 2:
        assert_string_equal(tags.title, "Loud 'n Proud");
        assert_uint64(
            get_duration_of_subtune(i),
            ==,
            TIME_TO_MS(0, 53, 0, 0)
        );
        break;
#endif
      default:
        break;
    }
  }
#endif
}
