project(
    'libgmtag',
    ['c', 'cpp'],
    version: '0.1',
    default_options: [
        'warning_level=3',
        'cpp_std=c++11',
        'cpp_eh=none',
        'c_std=c99',
        # 'b_sanitize=address,undefined',
        # 'b_coverage=true',
    ],
)

libgmtag_sources = [
    'src/libgmtag.cpp',
    'src/private/utils.cpp',
    'src/private/getters.cpp',
    'src/private/setters.cpp',
]

test_sources = [
    'src/tests/test.c',
    'src/tests/test_suite_1.c',
    'src/tests/test_suite_2.c',
]

fuzz_sources = [
    'src/tests/fuzz.c',
]

munit = dependency('munit', fallback: ['munit', 'munit_dep'])

libgmtag_includes = include_directories('include')

test_includes = include_directories('include', 'include/tests')

fuzz_includes = include_directories('include', 'include/fuzz')

libgmtag = static_library(
    'gmtag',
    libgmtag_sources,
    include_directories: libgmtag_includes,
    override_options: ['optimization=3'],
)

cc = meson.get_compiler('c')

libgmtag_nim = cc.find_library(
    'libgmtag', required: true, dirs: [
        meson.current_source_dir() + '/../libgmtag.nim'
    ]
)

executable(
    'libgmtag_test',
    test_sources,
    link_with: libgmtag,
    include_directories: [libgmtag_includes, test_includes],
    dependencies: [munit],
)

executable(
    'libgmtag_fuzz',
    fuzz_sources,
    link_with: libgmtag,
    include_directories: [libgmtag_includes, fuzz_includes],
)

executable(
    'libgmtag.nim_test',
    test_sources,
    include_directories: [libgmtag_includes, test_includes],
    dependencies: [munit, libgmtag_nim],
)

executable(
    'libgmtag.nim_fuzz',
    fuzz_sources,
    include_directories: [libgmtag_includes, fuzz_includes],
    dependencies: [libgmtag_nim],
)

