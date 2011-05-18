#include "bounded_buffer.h"

buffer*  
bounded_buffer_new(){
	buffer *buf;
	buf = malloc(sizeof(buffer));
	if(buf == NULL){
		perror("Error in malloc");
		exit(-1);
	}
	sem_unlink(BUFFER_MUTEX);
	buf->mutex = sem_open(BUFFER_MUTEX, O_CREAT, 0666,1);
	buf->last_pos  =0;
	buf->first_pos =0;
	buf->nb_elem   =0;
	buf->size = BUFFER_LENGTH;
	return buf;
}

buffer*
bounded_buffer_proc_new(key_t key){
	buffer *r, *buf;
	int shmid;                                   

    if ((shmid = shmget(key, sizeof(buffer*), IPC_CREAT | 0666)) < 0) {    
    	perror("shmget");                                              
		exit(-1);                                                      
	}                                                                  
	if ((r = shmat(shmid, NULL, 0)) == (buffer*) -1) {               
	    perror("shmat");                                               
	    exit(-1);                                                      
	}                                                                  
	buf = r;
	sem_unlink(BUFFER_MUTEX);
	buf->mutex = sem_open(BUFFER_MUTEX, O_CREAT, 0666, 1);
	buf->last_pos = 0;
	buf->first_pos =0;
	buf->nb_elem = 0;
	buf->size = BUFFER_LENGTH;
	return buf;                                                          
}


int
bounded_buffer_get(buffer *buf, char p[]){
	int i;
  	if(sem_wait(buf->mutex)==-1)
	  perror("mutex wait failed");
	if(buf->nb_elem == 0){
		if(sem_post(buf->mutex)==-1){
			perror("sem_wait error");
			exit(-1);
		}
		return BUFFER_EMPTY;
	}
	i = 0;
	while(buf->array[buf->first_pos][i] !='\0'){
		p[i]= buf->array[buf->first_pos][i];
		i++;
	}
	p[i] = '\0';
	buf->first_pos = (buf->first_pos + 1) % buf->size;
	buf->nb_elem--;
	if(sem_post(buf->mutex))
		perror("sem_post failed");
	return BUFFER_SUCCESS;
}


int
bounded_buffer_put(buffer *buf, char str[]){
  	unsigned int i;
	if(sem_wait(buf->mutex)==-1)
		perror("sem_post error");
  	if(buf->nb_elem == buf->size){
	  	if(sem_post(buf->mutex)==-1)
			perror("sem_wait error");
		return BUFFER_FULL;
	}
	/*buf->array[buf->last_pos % buf->size] = str;*/
	for(i=0; i<strlen(str); i++){
		buf->array[buf->last_pos % buf->size][i] = str[i];
	}
	buf->array[buf->last_pos % buf->size][i+1] = '\0';
	buf->last_pos = (buf->last_pos + 1) % buf->size;
	buf->nb_elem++;
	if(sem_post(buf->mutex) == -1)
	  	perror("sem_post error");
	return BUFFER_SUCCESS;
}

void
bounded_buffer_proc_free(key_t key){
	int id;

	if ((id = shmget(key, sizeof(buffer*), IPC_CREAT | 0666)) < 0) {
		perror("shmget");
		exit(-1);
	}

	if(shmctl(id, IPC_RMID,NULL) <0 )
        perror("error when detaching shared mem\n");
}

void 
bounded_buffer_free(buffer *buf){
	free(buf);
	sem_unlink(BUFFER_MUTEX);
}
