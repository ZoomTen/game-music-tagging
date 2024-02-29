project(
    'libgmtag',
    'cpp',
    version: '0.1',
    default_options: [
        'warning_level=3',
        'cpp_std=c++11'
    ]
)

libgmtag_sources = [
    'src/libgmtag.cpp'
]

test_sources = [
    'src/test.cpp',
    libgmtag_sources
]

executable(
    'libgmtag_test',
    test_sources,
    include_directories: ['include']
)