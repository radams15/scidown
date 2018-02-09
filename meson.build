project('Scidown', 'c',
  version : '2018.01.09',
  license : 'GPL3'
)

PREFIX = get_option('prefix')
PROJECT_NAME = 'scidown'
DATA_DIR = join_paths(PREFIX, 'share')
APP_DIR = join_paths(DATA_DIR, 'scidown')
SCRIPTS_DIR = join_paths(APP_DIR, 'scripts')

add_global_arguments(
  '-DSCRIPTS_DIR="@0@/"'.format(SCRIPTS_DIR),
  language : 'c'
)


lib_sources = [
    'src/autolink.c',
    'src/buffer.c',
    'src/document.c',
    'src/escape.c',
    'src/html_blocks.c',
    'src/html.c',
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

shared_library(
    PROJECT_NAME,
    sources: lib_sources,
    link_args: '-lm',
    install: true
)

executable(
    PROJECT_NAME,
    sources: [lib_sources, bin_sources],
    link_args: '-lm',
    c_args: ['-I../src/'],
    install: true
)

install_subdir(
  'resources/scripts',
  install_dir : APP_DIR
)
