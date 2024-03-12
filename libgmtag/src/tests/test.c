#include "munit.h"
#include "test_suite_1.h"

// clang-format off
static MunitTest basic_tests[] = {
    {"/Get subtune count",
        test_subtune_count,
        init_tags, deinit_tags,
        MUNIT_TEST_OPTION_NONE, NULL
    },
    {"/Get subtune order",
        test_subtune_order,
        init_tags, deinit_tags,
        MUNIT_TEST_OPTION_NONE, NULL
    },
    {"/Load subtune 1",
        test_subtune_1,
        init_tags, deinit_tags,
        MUNIT_TEST_OPTION_NONE, NULL
    },
    {"/Load subtune 2",
        test_subtune_2,
        init_tags, deinit_tags,
        MUNIT_TEST_OPTION_NONE, NULL
    },
    {"/Load subtune 3",
        test_subtune_3,
        init_tags, deinit_tags,
        MUNIT_TEST_OPTION_NONE, NULL
    },
    {"/Load subtune 4",
        test_subtune_4,
        init_tags, deinit_tags,
        MUNIT_TEST_OPTION_NONE, NULL
    },
    {0,0,0,0,0,0}
};

static MunitSuite basic[] = {
    {"/Basic with Reload", basic_tests, NULL, 0, MUNIT_SUITE_OPTION_NONE},
    {0,0,0,0,0}
};
// clang-format on

static const MunitSuite suite =
    {"/libgmtag", NULL, &basic, 0, MUNIT_SUITE_OPTION_NONE};

int main (int argc, char **argv) {
  return munit_suite_main(&suite, "munit", argc, argv);
}
