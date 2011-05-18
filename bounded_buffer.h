#ifndef __BOUNDED_BUFFER_H__
#define __BOUNDED_BUFFER_H__

#define BUFFER_FULL -1;
#define BUFFER_SUCCESS 0;
#define BUFFER_EMPTY -1;
#define BUFFER_LENGTH 10
#define BUFFER_PASS_LENGTH 50
#define BUFFER_MUTEX "bufmutex"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <semaphore.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>

#include "zip_crack.h"

struct buffer{
	char array[BUFFER_LENGTH][BUFFER_PASS_LENGTH];
	unsigned int first_pos;
	unsigned int last_pos;
	unsigned int nb_elem;
	unsigned int size;
	sem_t *mutex;
};
typedef struct buffer buffer;

/*
* Creates a bounded buffer of size BUFFER_LENGTH
* @return  buffer : a pointer to the struct bounded buffer
*/
buffer* bounded_buffer_new();

buffer* bounded_buffer_proc_new(key_t key);
/*
* Adds the char* str to the boundedBuffer bb
* Returns 0 if item successfuly added
* Returns -1 if failed i.e. the buffer is full
*/
int bounded_buffer_put(buffer *buf,  char* str);

/* 
* Removes and return the first added entry in the buffer
* returns NULL if buffer empty
* @param buf the buffer
*/
int  bounded_buffer_get(buffer *buf, char* p);

/*
* Frees the allocalted mem for the buffer
* Frees also each char* in it
*/
void bounded_buffer_free(buffer *buf);

/*
 * Frees the shared buffer with key key
 */
void bounded_buffer_proc_free(key_t key);



#endif


