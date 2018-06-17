CC ?= clang
DEBUG ?= 0
PROFILE ?= 0

rwildcard = $(foreach d,$(wildcard $1*),$(call rwildcard,$d/,$2) $(filter $(subst *,%,$2),$d))

FOUNDATION_SRCS = $(call rwildcard, bes/, *.c)
FOUNDATION_OBJS = $(FOUNDATION_SRCS:.c=.o)
FOUNDATION_DEPS = $(FOUNDATION_SRCS:.c=.d)

TEST_SRCS = $(call rwildcard, tests/, *.c)
TEST_OBJS = $(TEST_SRCS:.c=.o)
TEST_DEPS = $(TEST_SRCS:.c=.d)

# Release builds /w most aggressive optimization flags
CFLAGS_RELEASE = \
	-O3 \
	-fomit-frame-pointer \
	-fno-stack-protector \
	-ffast-math

# Profile builds /w slightly less aggressive optimization and /w debug symbols and
# disables Cef and Leap motion (as they fork the process which gprof cannot deal with)
CFLAGS_PROFILE = \
	-D_NDEBUG \
	-g3 \
	-O2 \
	-pg \
	-no-pie \
	-fno-inline-functions \
	-fno-inline-functions-called-once \
	-fno-optimize-sibling-calls

# Debug builds enable trap and stack protector /w Leap and Cef enabled
CFLAGS_DEBUG = \
	-g3 \
	-O0 \
	-ftrapv \
	-fstack-protector-all \

CFLAGS_COMMON = \
	-I. \
	-fPIC \
	-fstrict-aliasing \
	-Wall \
	-Wextra \
	-Wundef \
	-Wshadow \
	-Wpointer-arith \
	-Wunreachable-code \
	-Wwrite-strings \
	-Winit-self \
	-MMD

TEST_CFLAGS = $(CFLAGS_COMMON) $(CFLAGS_RELEASE)
TEST_BIN = test

FOUNDATION_CFLAGS = $(CFLAGS_COMMON) $(CFLAGS_RELEASE)
FOUNDATION_BIN = bes-foundation.a

ifeq ($(PROFILE),1)
FOUNDATION_CFLAGS = $(CFLAGS_COMMON) $(CFLAGS_PROFILE)
FOUNDATION_BIN = bes-foundation.a
endif

ifeq ($(DEBUG), 1)
FOUNDATION_CFLAGS = $(CFLAGS_COMMON) $(CFLAGS_DEBUG)
FOUNDATION_BIN = bes-foundation.a
endif

all: $(FOUNDATION_BIN)

bes/foundation/%.o: bes/foundation/%.c
	$(CC) $(FOUNDATION_CFLAGS) -c -o $@ $<

tests/%.o: tests/%.c
	$(CC) $(TEST_CFLAGS) -c -o $@ $<

$(FOUNDATION_BIN): $(FOUNDATION_OBJS)
	$(AR) -r -o $@ $^

$(TEST_BIN): $(TEST_OBJS) $(FOUNDATION_BIN)
	$(CC) -o $@ $^

clean:
	rm -rf $(FOUNDATION_OBJS) $(FOUNDATION_DEPS) $(FOUNDATION_BIN)
	rm -rf $(TEST_OBJS) $(TEST_DEPS) $(TEST_BIN)

.PHONY: clean

-include $(FOUNDATION_DEPS)
-include $(TEST_DEPS)
