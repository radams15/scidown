CFLAGS = -g -O3 -ansi -pedantic -Wall -Wextra -Wno-unused-parameter
PREFIX = /usr/local
BINDIR = $(PREFIX)/bin
LIBDIR = $(PREFIX)/lib
INCLUDEDIR = $(PREFIX)/include
YYPREFIX = mtex2MML_yy
LDFLAGS = -lm


SCIDOWN_CFLAGS = $(CFLAGS) -Isrc
ifneq ($(OS),Windows_NT)
	SCIDOWN_CFLAGS += -fPIC
endif

SONAME = -soname
ifeq ($(shell uname -s),Darwin)
	SONAME = -install_name
endif

SCIDOWN_SRC=\
	src/autolink.o \
	src/buffer.o \
	src/document.o \
	src/escape.o \
	src/html.o \
	src/html_blocks.o \
	src/html_smartypants.o \
	src/stack.o \
	src/version.o \
	src/charter/svg_utils.o \
	src/charter/renderer.o \
	src/charter/parser.o \
	src/charter/clist.o \
	src/charter/charter.o \
	src/charter/tinyexpr/tinyexpr.o \
	src/charter/csv_parser/csvparser.o

.PHONY:		all test test-pl clean

all:		libscidown.so libscidown.a scidown smartypants

# Libraries

libscidown.so: libscidown.so.3
	ln -f -s $^ $@

libscidown.so.3: $(SCIDOWN_SRC)
	$(CC) -Wl,$(SONAME),$(@F) -shared $^ $(LDFLAGS) -o $@

libscidown.a: $(SCIDOWN_SRC)
	$(AR) rcs libscidown.a $^

# Executables

scidown: bin/scidown.o $(SCIDOWN_SRC)
	$(CC) $^ $(LDFLAGS) -o $@

smartypants: bin/smartypants.o $(SCIDOWN_SRC)
	$(CC) $^ $(LDFLAGS) -o $@

# Perfect hashing

src/html_blocks.c: html_block_names.gperf
	gperf -L ANSI-C -N hoedown_find_block_tag -c -C -E -S 1 --ignore-case -m100 $^ > $@

# Testing

test: scidown
	python test/runner.py

test-pl: scidown
	perl test/MarkdownTest_1.0.3/MarkdownTest.pl \
		--script=./hoedown --testdir=test/MarkdownTest_1.0.3/Tests --tidy

# Housekeeping

clean:
	$(RM) src/*.o bin/*.o
	$(RM) libscidown.so libscidown.so.3 libscidown.a
	$(RM) scidown smartypants hoedown.exe smartypants.exe

# Installing

install:
	install -m755 -d $(DESTDIR)$(LIBDIR)
	install -m755 -d $(DESTDIR)$(BINDIR)
	install -m755 -d $(DESTDIR)$(INCLUDEDIR)

	install -m644 libhoedown.a $(DESTDIR)$(LIBDIR)
	install -m755 libhoedown.so.3 $(DESTDIR)$(LIBDIR)
	ln -f -s libhoedown.so.3 $(DESTDIR)$(LIBDIR)/libhoedown.so

	install -m755 hoedown $(DESTDIR)$(BINDIR)
	install -m755 smartypants $(DESTDIR)$(BINDIR)

	install -m755 -d $(DESTDIR)$(INCLUDEDIR)/hoedown
	install -m644 src/*.h $(DESTDIR)$(INCLUDEDIR)/hoedown

# Generic object compilations

%.o: %.c
	$(CC) $(SCIDOWN_CFLAGS) -c -o $@ $<

%.c: %.y
	bison -y -v -p $(YYPREFIX) -d -o $@ $<

%.c: %.l
	flex -P $(YYPREFIX) -o $@ $<

src/html_blocks.o: src/html_blocks.c
	$(CC) $(SCIDOWN_CFLAGS) -Wno-static-in-inline -c -o $@ $<
