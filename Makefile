CFLAGS=-O0 -g -Wall -Wextra -Wno-unused-parameter -Wno-deprecated-declarations -Iinclude
CPPFLAGS=-MMD
LDFLAGS=
LDLIBS=

OBJS=src/my_allocator.o
DEPS=$(OBJS:.o=.d)

.PHONY: all clean

all: test_hook test_allocator

-include $(DEPS)

%.o: %.c
	gcc -c $(CFLAGS) $(CPPFLAGS) $< -o $@

test_allocator: $(OBJS) tests/test_allocator.o
	gcc $(CFLAGS) $(CPPFLAGS) $(LDFLAGS) $^ $(LDLIBS) -o $@

test_hook: $(OBJS) tests/test_hook.o
	gcc $(CFLAGS) $(CPPFLAGS) $(LDFLAGS) $^ $(LDLIBS) -o $@

clean:
	rm -f $(OBJS) $(DEPS) test_allocator test_hook tests/test_allocator.o tests/test_hook.o tests/test_allocator.d tests/test_hook.d