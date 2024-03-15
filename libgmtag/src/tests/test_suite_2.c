#define MUNIT_ENABLE_ASSERT_ALIASES
#define UNUSED(x) (void)(x)
#include "munit.h"
#include "test_suite_2.h"
#include "libgmtag.h"

#include "./sample_data.h"

#define TIME_TO_MS(h, m, s, ms) \
  (h * 60 * 60 * 1000) + (m * 60 * 1000) + (s * 1000) + (ms)

MunitResult test_tags_after_reload (
    const MunitParameter params[],
    void *data
) {
  UNUSED(params);
  UNUSED(data);

  // simulate reloading tracks
  tags_from_buffer(tag_init_data);
  tags_from_buffer(katakis_3d_test);

  for (size_t i = 1; i <= 18; i++) {
    assert_uint64(get_track_num(i), ==, i);

    // check global tags
    char *album = get_album(i);
    char *company = get_company(i);
    char *ripper = get_ripper(i);
    char *tagger = get_tagger(i);

    assert_string_equal(album, "Katakis 3D");
    assert_string_equal(company, "Similis");
    assert_string_equal(ripper, "zlago");
    assert_string_equal(tagger, "Zumi, zlago");

    free(album);
    free(company);
    free(ripper);
    free(tagger);

    assert_uint64(get_date(i).year, ==, 2001);

    // check artist
    char *artist = get_artist(i);
    switch (i) {
      case 1:
        assert_string_equal(
            artist,
            "Chris Huelsbeck, Tufan Uysal"
        );
        break;
      default:
        assert_string_equal(artist, "Tufan Uysal");
        break;
    }
    free(artist);

    char *title = get_title(i);
    // check other metadata
    switch (i) {
      case 1:
        assert_string_equal(title, "Katakis (Remix)");
        assert_uint64(
            get_duration_of_subtune(i),
            ==,
            TIME_TO_MS(0, 3, 17, 0)
        );
        break;
      case 2:
        assert_string_equal(title, "Loud 'n Proud");
        assert_uint64(
            get_duration_of_subtune(i),
            ==,
            TIME_TO_MS(0, 0, 53, 0)
        );
        break;
      case 3:
        assert_string_equal(title, "Flight to Hell");
        assert_uint64(
            get_duration_of_subtune(i),
            ==,
            TIME_TO_MS(0, 1, 18, 0)
        );
        break;
      case 4:
        assert_string_equal(title, "The Big Thing");
        assert_uint64(
            get_duration_of_subtune(i),
            ==,
            TIME_TO_MS(0, 0, 49, 0)
        );
        break;
      case 5:
        assert_string_equal(title, "Electrical Motions");
        assert_uint64(
            get_duration_of_subtune(i),
            ==,
            TIME_TO_MS(0, 1, 31, 0)
        );
        break;
      case 6:
        assert_string_equal(title, "Protected Beat");
        assert_uint64(
            get_duration_of_subtune(i),
            ==,
            TIME_TO_MS(0, 0, 24, 0)
        );
        break;
      case 7:
        assert_string_equal(title, "Secret Cycles");
        assert_uint64(
            get_length_of_subtune(i),
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
            TIME_TO_MS(0, 2, 2, 0)
        );
        break;
      case 8:
        assert_string_equal(title, "Radioactive Attack");
        assert_uint64(
            get_duration_of_subtune(i),
            ==,
            TIME_TO_MS(0, 0, 42, 0)
        );
        break;
      case 9:
        assert_string_equal(title, "Enforcer");
        assert_uint64(
            get_duration_of_subtune(i),
            ==,
            TIME_TO_MS(0, 1, 30, 0)
        );
        break;
      case 10:
        assert_string_equal(title, "Someone Wanna Party");
        assert_uint64(
            get_duration_of_subtune(i),
            ==,
            TIME_TO_MS(0, 0, 50, 0)
        );
        break;
      case 11:
        assert_string_equal(title, "Rasit's Spiritual Dreams");
        assert_uint64(
            get_duration_of_subtune(i),
            ==,
            TIME_TO_MS(0, 1, 18, 0)
        );
        break;
      case 12:
        assert_string_equal(title, "Oriental Danger");
        assert_uint64(
            get_duration_of_subtune(i),
            ==,
            TIME_TO_MS(0, 0, 50, 0)
        );
        break;
      case 13:
        assert_string_equal(title, "Boomin' Back Katakis");
        assert_uint64(
            get_duration_of_subtune(i),
            ==,
            TIME_TO_MS(0, 4, 1, 0)
        );
        break;
      case 14:
        assert_string_equal(title, "Master of Universe");
        assert_uint64(
            get_duration_of_subtune(i),
            ==,
            TIME_TO_MS(0, 1, 14, 0)
        );
        break;
      case 15:
        assert_string_equal(title, "The Impregnable");
        assert_uint64(
            get_duration_of_subtune(i),
            ==,
            TIME_TO_MS(0, 1, 1, 0)
        );
        break;
      case 16:
        assert_string_equal(title, "30 Seconds to Go...");
        assert_uint64(
            get_duration_of_subtune(i),
            ==,
            TIME_TO_MS(0, 0, 45, 0)
        );
        break;
      case 17:
        assert_string_equal(title, "Beyond the Stars");
        assert_uint64(
            get_duration_of_subtune(i),
            ==,
            TIME_TO_MS(0, 4, 27, 0)
        );
        break;
      case 18:
        assert_string_equal(title, "Crush Boom Bang");
        assert_uint64(
            get_duration_of_subtune(i),
            ==,
            TIME_TO_MS(0, 0, 7, 0)
        );
        break;
      default:
        break;
    }

    free(title);
  }

  return MUNIT_OK;
}
