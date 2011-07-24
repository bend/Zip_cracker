#include "file_reader.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

FILE* 
open_file(const char* path){
	FILE* f;
		if((f=fopen(path, "r"))==NULL){
			perror("cannot open file");
			exit(-1);
		}	
		return f;
}

int close_file(FILE* file){
	return fclose(file);
}

int
get_next(FILE* f, char* pass){
	int length;
		pass[0] = '\0';					/*Put \O to see if we read a character in the file and return -1 if not*/
		fgets(pass,50,f);
		length = strlen(pass);
		if(length <2)
			return -1;
		pass[length-1] = '\0';
		return 0;
}
