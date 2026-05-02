# Makefile — Janelas Para C windowing library
#
# Targets:
#   make           build the demo (default)
#   make run       build and run the demo
#   make clean     remove built files

CC      = gcc
# -x c  forces C compilation for .C files (uppercase extension defaults to C++)
CFLAGS  = -Wall -Wextra -std=c99 -D_XOPEN_SOURCE_EXTENDED -x c
LDFLAGS = -lncursesw

# Object files that make up the library + demo
OBJS = SAMPLE.o WINDOWS.o TEXT.o winc_compat.o

.PHONY: all run clean

all: sample

sample: $(OBJS)
	$(CC) -o $@ $^ $(LDFLAGS)

SAMPLE.o: SAMPLE.C WINC.H winc_compat.h
	$(CC) $(CFLAGS) -c -o $@ $<

WINDOWS.o: WINDOWS.C WINC.H winc_compat.h
	$(CC) $(CFLAGS) -c -o $@ $<

TEXT.o: TEXT.C WINC.H winc_compat.h
	$(CC) $(CFLAGS) -c -o $@ $<

winc_compat.o: winc_compat.c winc_compat.h
	$(CC) $(CFLAGS) -c -o $@ $<

run: sample
	./sample

clean:
	rm -f $(OBJS) sample
