project('Scidown', 'c',
  version : '2018.01.09',
  license : 'GPL3'
)

PROJECT_NAME = 'scidown'

lib_sources = [
    'src/utils.c',
    'src/autolink.c',
    'src/buffer.c',
    'src/document.c',
    'src/escape.c',
    'src/html_blocks.c',
    'src/html.c',
    'src/latex.c',
    'src/html_smartypants.c',
    'src/stack.c',
    'src/version.c',
    'src/charter/clist.c',
    'src/charter/parser.c',
    'src/charter/charter.c',
    'src/charter/renderer.c',
    'src/charter/svg_utils.c',
    'src/charter/tinyexpr/tinyexpr.c',
    'src/charter/csv_parser/csvparser.c'
]


bin_sources = [
  'bin/scidown.c'
]

deps = []

shared_library(
    PROJECT_NAME,
    sources: lib_sources,
    link_args: '-lm',
    dependencies : deps,
    install: true
)

executable(
    PROJECT_NAME,
    sources: [lib_sources, bin_sources],
    link_args: '-lm',
    c_args: ['-I../src/'],
    dependencies : deps,
    install: true
)
