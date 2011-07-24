#include <string.h>
#include <stdio.h>

#include "cracker.h"
#include <getopt.h>

void
usage (void)
{
    printf("Usage: [zipcrack [-t | -p ] <NB> <-d DICTINARY> <ZIP_FILE>]\n");
}


void checkParams(char* file, char* pflag, char* tflag, char* dflag){
    if(dflag ==NULL){
		usage();
		exit(-1);
    }if(pflag!=NULL && tflag!=NULL){
        usage();
        exit(-1);
    }else if(pflag==NULL && tflag==NULL){
        usage();
        exit(-1);
    }else if(file == NULL){
        usage();
        exit(-1);
    }
}

int 
main (int argc, char ** argv)
{
 
 int c;
    char* pflag = 0;
    char* tflag = 0;
    char* dflag = 0;
    char* file;

        if (argc < 2) {
            usage();
            return 1;
        }

        while ((c = getopt (argc, argv, "p:t:d:")) != -1) {  /*Parse arguments*/
            switch (c){
                case 'p':
                    pflag = (char*)optarg;
                    break;
                case 't':
                    tflag = (char*)optarg;
                    break;
				case 'd':
		    		dflag = (char*)optarg;
		    		break;
            }
        }
       
        file = argv[argc-1];
		checkParams(file, pflag, tflag,dflag); /*Exits if arguments are wrong*/
	
	if(pflag){
	  	printf("Password cracking of archive %s with dictionary %s started using %d process\n",file,dflag,atoi(pflag));
	    start_cracking('p',atoi(pflag), file, dflag);

	}else{
	  	printf("Password cracking of archive %s with dictionary %s started using %d threads\n",file,dflag,atoi(tflag));
	    start_cracking('t',atoi(tflag), file, dflag);
	}
  
 	return 1;
}
