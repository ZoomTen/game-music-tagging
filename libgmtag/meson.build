project(
    'libgmtag',
    ['c', 'cpp'],
    version: '0.1',
    default_options: [
        'warning_level=3',
        'cpp_std=c++11',
        'c_std=c99'
    ]
)

libgmtag_sources = [
    'src/libgmtag.cpp'
]

test_sources = [
    'src/tests/test.c',
    'src/tests/test_suite_1.c',
    libgmtag_sources
]

munit = dependency('munit', fallback: ['munit', 'munit_dep'])

includes = include_directories('include', 'include/tests')

executable(
    'libgmtag_test',
    test_sources,
    include_directories: includes,
    dependencies: munit
)
