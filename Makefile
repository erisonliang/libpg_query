root_dir := $(shell dirname $(realpath $(lastword $(MAKEFILE_LIST))))

TARGET = pg_query
ARLIB = lib$(TARGET).a
PGDIR = $(root_dir)/tmp/postgres
PGDIRBZ2 = $(root_dir)/tmp/postgres.tar.bz2

PG_VERSION = 12.3

IGNORED_PROTOBUF_FILES := $(wildcard protobuf/protobuf-v3.12.0/google/protobuf/*test.cc) \
$(wildcard protobuf/protobuf-v3.12.0/google/protobuf/test_*.cc) \
$(wildcard protobuf/protobuf-v3.12.0/google/protobuf/unittest_*.cc) \
protobuf/protobuf-v3.12.0/google/protobuf/map_lite_test_util.cc

SRC_FILES := $(wildcard src/*.c src/postgres/*.c) protobuf-c/protobuf-c.c protobuf/scan_output.pb-c.c
CXX_SRC_FILES := $(wildcard src/*.cc) protobuf/parse_tree.pb.cc \
$(filter-out $(IGNORED_PROTOBUF_FILES), $(wildcard protobuf/protobuf-v3.12.0/google/protobuf/*.cc)) \
$(filter-out $(wildcard protobuf/protobuf-v3.12.0/google/protobuf/stubs/*test.cc), $(wildcard protobuf/protobuf-v3.12.0/google/protobuf/stubs/*.cc)) \
$(filter-out $(wildcard protobuf/protobuf-v3.12.0/google/protobuf/io/*test.cc), $(wildcard protobuf/protobuf-v3.12.0/google/protobuf/io/*.cc)) \
$(filter-out $(wildcard protobuf/protobuf-v3.12.0/google/protobuf/util/*test.cc), $(wildcard protobuf/protobuf-v3.12.0/google/protobuf/util/*.cc)) \
$(filter-out $(wildcard protobuf/protobuf-v3.12.0/google/protobuf/util/internal/*test.cc), $(wildcard protobuf/protobuf-v3.12.0/google/protobuf/util/internal/*.cc))
OBJ_FILES := $(SRC_FILES:.c=.o) $(CXX_SRC_FILES:.cc=.o)
NOT_OBJ_FILES := src/pg_query_fingerprint_defs.o src/pg_query_fingerprint_conds.o src/pg_query_json_defs.o src/pg_query_json_conds.o src/pg_query_protobuf_defs.o src/pg_query_protobuf_conds.o src/postgres/guc-file.o src/postgres/scan.o src/pg_query_json_helper.o
OBJ_FILES := $(filter-out $(NOT_OBJ_FILES), $(OBJ_FILES))

CFLAGS  = -I. -I./src/postgres/include -Wall -Wno-unused-function -Wno-unused-value -Wno-unused-variable -fno-strict-aliasing -fwrapv -fPIC
CXXFLAGS = -I. -I./src/postgres/include -I./protobuf/protobuf-v3.12.0 -DHAVE_PTHREAD -std=c++11 -Wall -pedantic -Wno-zero-length-array -Wno-c99-extensions -fwrapv -fPIC
LIBPATH = -L.

PG_CONFIGURE_FLAGS = -q --without-readline --without-zlib
PG_CFLAGS = -fPIC

ifeq ($(DEBUG),1)
	CFLAGS += -O0 -g
	CXXFLAGS += -O0 -g
	PG_CONFIGURE_FLAGS += --enable-cassert --enable-debug
else
	CFLAGS += -O3 -g
	CXXFLAGS += -O3 -g
	PG_CFLAGS += -O3
endif

CLEANLIBS = $(ARLIB)
CLEANOBJS = $(OBJ_FILES)
CLEANFILES = $(PGDIRBZ2)

AR = ar rs
RM = rm -f
ECHO = echo

CC ?= cc

all: examples test build

build: $(ARLIB)

clean:
	-@ $(RM) $(CLEANLIBS) $(CLEANOBJS) $(CLEANFILES) $(EXAMPLES) $(TESTS)
	-@ $(RM) -rf {test,examples}/*.dSYM
	-@ $(RM) -r $(PGDIR) $(PGDIRBZ2)

.PHONY: all clean build extract_source examples test

$(PGDIR):
	curl -o $(PGDIRBZ2) https://ftp.postgresql.org/pub/source/v$(PG_VERSION)/postgresql-$(PG_VERSION).tar.bz2
	tar -xjf $(PGDIRBZ2)
	mv $(root_dir)/postgresql-$(PG_VERSION) $(PGDIR)
	cd $(PGDIR); patch -p1 < $(root_dir)/patches/01_parse_replacement_char.patch
	cd $(PGDIR); patch -p1 < $(root_dir)/patches/02_lexer_track_yyllocend.patch
	cd $(PGDIR); patch -p1 < $(root_dir)/patches/03_comment_scanner.patch
	cd $(PGDIR); CFLAGS="$(PG_CFLAGS)" ./configure $(PG_CONFIGURE_FLAGS)
	cd $(PGDIR); rm src/pl/plpgsql/src/pl_gram.h
	cd $(PGDIR); make -C src/pl/plpgsql/src pl_gram.h
	cd $(PGDIR); make -C src/port pg_config_paths.h
	cd $(PGDIR); make -C src/backend generated-headers
	cd $(PGDIR); make -C src/backend parser-recursive # Triggers copying of includes to where they belong, as well as generating gram.c/scan.c

extract_source: $(PGDIR)
	-@ $(RM) -rf ./src/postgres/
	mkdir ./src/postgres
	mkdir ./src/postgres/include
	LIBCLANG=/Library/Developer/CommandLineTools/usr/lib/libclang.dylib ruby ./scripts/extract_source.rb $(PGDIR)/ ./src/postgres/
	cp $(PGDIR)/src/include/storage/dsm_impl.h ./src/postgres/include/storage
	touch ./src/postgres/guc-file.c
	# This causes compatibility problems on some Linux distros, with "xlocale.h" not being available
	echo "#undef HAVE_LOCALE_T" >> ./src/postgres/include/pg_config.h
	echo "#undef LOCALE_T_IN_XLOCALE" >> ./src/postgres/include/pg_config.h
	echo "#undef WCSTOMBS_L_IN_XLOCALE" >> ./src/postgres/include/pg_config.h
	# Support 32-bit systems without reconfiguring
	echo "#undef PG_INT128_TYPE" >> ./src/postgres/include/pg_config.h
	# Support gcc earlier than 4.6.0 without reconfiguring
	echo "#undef HAVE__STATIC_ASSERT" >> ./src/postgres/include/pg_config.h
	# Copy version information so its easily accessible
	sed -i "" '$(shell echo 's/\#define PG_MAJORVERSION .*/'`grep "\#define PG_MAJORVERSION " ./src/postgres/include/pg_config.h`'/')' pg_query.h
	sed -i "" '$(shell echo 's/\#define PG_VERSION .*/'`grep "\#define PG_VERSION " ./src/postgres/include/pg_config.h`'/')' pg_query.h
	sed -i "" '$(shell echo 's/\#define PG_VERSION_NUM .*/'`grep "\#define PG_VERSION_NUM " ./src/postgres/include/pg_config.h`'/')' pg_query.h

.c.o:
	@$(ECHO) compiling $(<)
	@$(CC) $(CPPFLAGS) $(CFLAGS) -o $@ -c $<

.cc.o:
	@$(ECHO) compiling $(<)
	@$(CXX) $(CXXFLAGS) -o $@ -c $<

$(ARLIB): $(OBJ_FILES) Makefile
	@$(AR) $@ $(OBJ_FILES)

protobuf/scan_output.pb-c.c: protobuf/scan_output.proto
	protoc --c_out=. protobuf/scan_output.proto

protobuf/parse_tree.pb.cc: protobuf/parse_tree.proto
	protoc --cpp_out=. protobuf/parse_tree.proto

EXAMPLES = examples/simple examples/scan examples/normalize examples/simple_error examples/normalize_error examples/simple_plpgsql
examples: $(EXAMPLES)
	examples/simple
	examples/scan
	examples/normalize
	examples/simple_error
	examples/normalize_error
	examples/simple_plpgsql

examples/simple: examples/simple.c $(ARLIB)
	$(CC) -I. -o $@ -g -lstdc++ examples/simple.c $(ARLIB)

examples/scan: examples/scan.c $(ARLIB)
	$(CC) -I. -o $@ -g -lstdc++ examples/scan.c $(ARLIB)

examples/normalize: examples/normalize.c $(ARLIB)
	$(CC) -I. -o $@ -g -lstdc++ examples/normalize.c $(ARLIB)

examples/simple_error: examples/simple_error.c $(ARLIB)
	$(CC) -I. -o $@ -g -lstdc++ examples/simple_error.c $(ARLIB)

examples/normalize_error: examples/normalize_error.c $(ARLIB)
	$(CC) -I. -o $@ -g -lstdc++ examples/normalize_error.c $(ARLIB)

examples/simple_plpgsql: examples/simple_plpgsql.c $(ARLIB)
	$(CC) -I. -o $@ -g -lstdc++ examples/simple_plpgsql.c $(ARLIB)

examples/protobuf_test: examples/protobuf_test.cc $(ARLIB)
	$(CXX) $(CXXFLAGS) -I. -o $@ examples/protobuf_test.cc $(ARLIB)

TESTS = test/complex test/concurrency test/fingerprint test/normalize test/parse test/parse_plpgsql test/scan
test: $(TESTS)
	test/complex
	test/concurrency
	test/fingerprint
	test/normalize
	test/parse
	test/scan
	# Output-based tests
	test/parse_plpgsql
	diff -Naur test/plpgsql_samples.expected.json test/plpgsql_samples.actual.json

test/complex: test/complex.c $(ARLIB)
	$(CC) -I. -Isrc -o $@ -g -lstdc++ test/complex.c $(ARLIB)

test/concurrency: test/concurrency.c test/parse_tests.c $(ARLIB)
	$(CC) -I. -o $@ -pthread -g -lstdc++ test/concurrency.c $(ARLIB)

test/fingerprint: test/fingerprint.c test/fingerprint_tests.c $(ARLIB)
	$(CC) -I. -Isrc -o $@ -g -lstdc++ test/fingerprint.c $(ARLIB)

test/normalize: test/normalize.c test/normalize_tests.c $(ARLIB)
	$(CC) -I. -Isrc -o $@ -g -lstdc++ test/normalize.c $(ARLIB)

test/parse: test/parse.c test/parse_tests.c $(ARLIB)
	$(CC) -I. -o $@ -g -lstdc++ test/parse.c $(ARLIB)

test/parse_plpgsql: test/parse_plpgsql.c $(ARLIB)
	$(CC) -I. -o $@ -I./src -I./src/postgres/include -g -lstdc++ test/parse_plpgsql.c $(ARLIB)

test/scan: test/scan.c test/scan_tests.c $(ARLIB)
	$(CC) -I. -o $@ -g -lstdc++ test/scan.c $(ARLIB)