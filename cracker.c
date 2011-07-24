#include "cracker.h"


void*
crack_password(void* arg){
  	params* p;
	buffer* buf;
	char pass[50];
	struct zip_archive * archive;
		p =(params*)arg;
		buf = p->buf;
		if ((archive = zip_load_archive(p->zipfile)) == NULL) {
			printf("Unable to open archive %s\n", p->zipfile);
			exit(-1);
		}
		do{	
		 	if (sem_wait(p->full)==-1){
			 	perror("error sem_wait on full w \n");
				exit(-1);
			}
			if(bounded_buffer_get(buf,pass) == 0){
				if(zip_test_password(archive, pass) == 0) {
					p->found = true;
					printf("###################################\n");
				  	printf("####[Password is: %s]\n", pass);
					printf("##################################\n");
				}
			}
			if(sem_post(p->empty)==-1){
			 	perror("error sem_post on full");
				exit(-1);
			}
		}while(!p->found);

		pthread_exit(NULL);
}


void*
fill_buffer(void* arg){
	params* p;
	buffer* buf;
	char temp[BUFFER_LENGTH];
	FILE* file;
		p=(params*)arg;
		buf=p->buf;
		file = open_file(p->dictionary);    /*            		  Open file        */
		do{
		  	if(sem_wait(p->empty)==-1)
			  	perror("error on sem empty");
			if(buf->nb_elem < buf->size){
				if(get_next(file, temp)<0){
					p->found = true;
					printf("Password not found\n");
				}
				bounded_buffer_put(buf, temp);
			}
			sem_post(p->full);
		}while(temp!=NULL && !p->found);
		close_file(file);
	pthread_exit(NULL);
}


void 
create_threads(unsigned int nb_threads, char* file_to_crack, char* dictionary_file){
	pthread_t *threads;										/*Array of threads*/
	int rc;
	unsigned int i;
	int pthread_join_res;
	params *crack_params;
		sem_unlink(SEM_FULL);
		sem_unlink(SEM_EMPTY);
		threads = malloc(sizeof(pthread_t)*nb_threads+1);			/* nb_threads+1 for storing the bounded_buffer thread*/
		if(threads == NULL){
			perror("malloc failed");
			exit(-1);
		}
		crack_params = malloc(sizeof(params));
		if(crack_params == NULL){
			perror("malloc failed");
			free(threads);
			exit(-1);
		}
		crack_params->buf = bounded_buffer_new();
		crack_params->zipfile= file_to_crack;
		crack_params->dictionary= dictionary_file;
		crack_params->empty = sem_open(SEM_EMPTY, O_CREAT, 0666, crack_params->buf->size);;
		crack_params->full = sem_open(SEM_FULL, O_CREAT, 0666, 0);
		crack_params->found = false;
		for(i=0; i<nb_threads; i++){						/* create crack threads */
			rc = pthread_create(&threads[i],NULL,crack_password,(void*)crack_params);   /*Creating thread*/
			if (rc){
				printf("ERROR; return code from pthread_create() is %d\n", rc);
			    free(threads);
				bounded_buffer_free(crack_params->buf);
				free(crack_params);
			    exit(-1);
			}
		}
		rc =pthread_create(&threads[nb_threads],NULL,fill_buffer,(void*)crack_params);
		if(rc){
			printf("ERROR; return code from pthread_create() is %d\n", rc);
            free(threads);
			bounded_buffer_free(crack_params->buf);
			free(crack_params);
			exit(-1);
		}
		for(i=0; i<nb_threads+1; i++)
			pthread_join_res = pthread_join(threads[i], NULL);
		bounded_buffer_free(crack_params->buf);
		free(crack_params);
		free(threads);
		sem_unlink(SEM_EMPTY);
		sem_unlink(SEM_FULL);
}


void create_process(unsigned int nb_process, char* file_to_crack, char* dictionary_file){
	params* shared;
	unsigned int i;
	pid_t *pids;
	sem_unlink(SEM_EMPTY);
	sem_unlink(SEM_FULL);
		pids = malloc(sizeof(pid_t)*(nb_process +1));
		if(pids == NULL){
			perror("malloc failed");
			exit(-1);
		}
		shared = create_mem_segment(PARAMS_KEY);
		shared->buf = bounded_buffer_proc_new(BB_KEY);
		shared->dictionary=dictionary_file;
		shared->zipfile = file_to_crack;
		shared->found = false;
		shared->empty = sem_open(SEM_EMPTY,O_CREAT,0666,shared->buf->size);  
		shared->full = sem_open(SEM_FULL,O_CREAT,0666,0);             
		for(i=0; i<nb_process+1; i++){
			pids[i]=fork();
			if(i==(nb_process)){
				if(pids[i]==0){
					fill_buffer(shared);
				}else if(pids[i]<0){
					perror("fork error");
					exit(-1);
				}
			}else{	
				if(pids[i]==0){
					crack_password(shared);
				}else if(pids[i]<0){
					perror("fork error");
					exit(-1);
				}
			}	  
		}
		for(i=0; i<nb_process; i++)
			waitpid(pids[i], NULL, 0);	
		sem_unlink(SEM_EMPTY);
		sem_unlink(SEM_FULL);
		bounded_buffer_proc_free(BB_KEY);
		free_mem_segment(PARAMS_KEY);
}


void start_cracking(char tp, unsigned int nb_pt, char* file_to_crack, char* dictionary_file){
		if(tp=='p')
			create_process(nb_pt, file_to_crack, dictionary_file);
		else
			create_threads(nb_pt, file_to_crack, dictionary_file);
}

params* create_mem_segment(key_t key ){
    int shmid;  
  	params *shm,*r;
		if ((shmid = shmget(key, sizeof(shm), IPC_CREAT | 0666)) < 0) {
			perror("shmget");
			exit(-1);
		}
		if ((shm = shmat(shmid, NULL, 0)) == (params*) -1) {
			perror("shmat");
			exit(-1);
		}
		r=shm;
		return r;
}


params*  get_mem_segment(key_t key){
	int shmid;
	params *shm,*r;
		
		if ((shmid = shmget(key,sizeof(params*) , 0666)) < 0) {
			perror("shmget");
			exit(1);
		}
		if ((shm = shmat(shmid, NULL, 0)) == (params *) -1) {
			perror("shmat");
			exit(1);
		}
		r=shm;
		return r;
}


void free_mem_segment(key_t key){
	int id;

	if ((id = shmget(key, sizeof(params*), IPC_CREAT | 0666)) < 0) {
		perror("shmget");
		exit(-1);
	}

	if(shmctl(id, IPC_RMID,NULL) < 0){
		perror("error while detaching shared mem");
	}
}
