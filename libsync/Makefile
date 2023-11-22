CC = gcc
CFLAGS = -O2 -g # -Wall
LDFLAGS = -g
SRCDIR  = .
INCLUDEDIRS = .

SOURCES  = array.c bigint.c
SOURCES += test.c

OBJECTS = $(SOURCES:.c=.o)
EXECUTABLE = test

.PHONY: all clean

all: $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(LDFLAGS) -o $@ $^

%.o: $(SRCDIR)/%.c
	$(CC) $(CFLAGS) $(foreach dir,$(INCLUDEDIRS),-I$(dir)) -c -o $@ $<

clean:
	rm -f $(EXECUTABLE) $(OBJECTS)
