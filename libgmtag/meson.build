project(
    'libgmtag',
    ['c', 'cpp'],
    version: '0.1',
    default_options: [
        'warning_level=everything',
        'cpp_std=c++11',
        'c_std=c99'
    ]
)

libgmtag_sources = [
    'src/libgmtag.cpp'
]

test_sources = [
    'src/test.c',
    libgmtag_sources
]

executable(
    'libgmtag_test',
    test_sources,
    include_directories: ['include']
)