# CC = gcc
# CFLAGS = -O1 -Wall
# LDFLAGS =
# SRCDIR  = .
# INCLUDEDIRS =  . ./iwflea/header
# INCLUDEDIRS += ./iwbwflea/header ./iwbwflea

# SOURCES =  ./iwflea/ariacbc.c ./iwflea/drbg.c ./iwflea/randperm.c ./iwflea/seedcbc.c ./iwflea/sha256.c ./iwflea/iwbwflea_encoding.c  ./iwflea/iwbwflea_tables.c ./iwflea/iwflea.c 
# SOURCES += ./iwflea/iwbwflea_ext_transform.c
# SOURCES += ./iwbwflea/iwbwflea.c 
# SOURCES += test.c

# OBJECTS = $(SOURCES:.c=.o)
# EXECUTABLE = test

# .PHONY: all clean

# all: $(EXECUTABLE)

# $(EXECUTABLE): $(OBJECTS)
# 	$(CC) $(LDFLAGS) -o $@ $^

# %.o: $(SRCDIR)/%.c
# 	$(CC) $(CFLAGS) $(foreach dir,$(INCLUDEDIRS),-I$(dir)) -c -o $@ $<

# clean:
# 	rm -f $(EXECUTABLE) $(OBJECTS)

CC = gcc
CFLAGS = -O1 -Wall
LDFLAGS =
SRCDIR  = .
INCLUDEDIRS =  .

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
