project(scidown C)

cmake_minimum_required(VERSION 2.8)

set(CHARTER_SRC 
	src/charter/src/clist.c
	src/charter/src/parser.c
	src/charter/src/charter.c
	src/charter/src/svg.c
	src/charter/src/latex.c
	src/charter/src/charter_string.c
	src/charter/src/svg_utils.c
	src/charter/src/tinyexpr/tinyexpr.c
	src/charter/src/csv_parser/csvparser.c
)

set(LIB_SRC
	src/utils.c
	src/constants.c
	src/autolink.c
	src/buffer.c
	src/document.c
	src/escape.c
	src/html_blocks.c
	src/html.c
	src/latex.c
	src/html_smartypants.c
	src/stack.c
	src/version.c
)

set(BIN_SRC
	bin/scidown.c
)

add_library(scidown SHARED ${CHARTER_SRC} ${LIB_SRC})

include_directories(src/)

install(TARGETS scidown DESTINATION lib)
