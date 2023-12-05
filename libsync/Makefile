CC = gcc
CFLAGS = -O2 -g # -Wall
LDFLAGS = -g
SRCDIR  = .
INCLUDEDIRS = ./include

LIBSOURCES = array.c bigint.c
LIBOBJECTS = $(LIBSOURCES:.c=.o)
LIBRARY = libsync.a

SOURCES = test.c
OBJECTS = $(SOURCES:.c=.o)
EXECUTABLE = test

.PHONY: all clean

all: $(EXECUTABLE)

$(EXECUTABLE): $(LIBRARY) $(OBJECTS)
	$(CC) $(LDFLAGS) -o $@ $^ -L. -lsync

$(LIBRARY): $(LIBOBJECTS)
	ar rcs $@ $^

%.o: $(SRCDIR)/%.c
	$(CC) $(CFLAGS) $(foreach dir,$(INCLUDEDIRS),-I$(dir)) -c -o $@ $<

clean:
	rm -f $(EXECUTABLE) $(LIBRARY) $(OBJECTS) $(LIBOBJECTS)
