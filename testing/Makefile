CC=gcc
CFLAGS=-W -Wall -Werror -ansi -pedantic 
EXEC = tests

all: $(EXEC)

$(EXEC): testing.o file_reader.o bounded_buffer.o
	$(CC) -o tests testing.o file_reader.o bounded_buffer.o 
file_reader.o: ../file_reader.c
	$(CC) -o file_reader.o -c ../file_reader.c $(CFLAGS)
bounded_buffer.o: ../bounded_buffer.c
	$(CC) -o bounded_buffer.o -c ../bounded_buffer.c $(CFLAGS)
testing.o: testing.c ../file_reader.h ../bounded_buffer.h 
	$(CC) -o testing.o -c testing.c $(CFLAGS)

.PHONY: clean mrproper
                            
clean:
	@rm *.o
mrproper: 
	@rm $(EXEC)

