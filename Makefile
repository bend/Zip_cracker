CFLAGS+=-W -Wall -Lpthread -Werror -ansi -pedantic  -std=gnu99
LDFLAGS+= -lpthread
EXEC=zipcrack
SOURCES=$(wildcard *.c)
OBJECTS=$(SOURCES:.c=.o)
CC=gcc

all: $(EXEC)
debug: CFLAGS+= -DDEBUG -g
debug: $(EXEC)

$(EXEC): $(OBJECTS)
	$(CC) -o $@ $(OBJECTS) $(LDFLAGS)

%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<

.PHONY: clean mrproper

clean:
	@rm $(OBJECTS)

mrproper:
	@rm $(EXEC)
