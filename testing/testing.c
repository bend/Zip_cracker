#include "../file_reader.h"
#include "../bounded_buffer.h"

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

void test_file_reader(){
	FILE* f;
	char pass[50];
		f=open_file("tests.txt");
		assert(get_next(f,pass)==0);
		assert(strcmp(pass,"line1")==0);
		assert(get_next(f,pass)==0);
		assert(strcmp(pass,"line2")==0);
		assert(get_next(f,pass)==0);
		assert(strcmp(pass, "line 3")==0);
		assert(get_next(f,pass)==0);
		assert(strcmp(pass, "line4 line5")==0);
		assert(get_next(f,pass)==0);
		assert(strcmp(pass, "1 2 3 4 5 6 7 8 9 10 11 12 13")==0);
		assert(get_next(f,pass)==-1);
}

void test_buffer(){
	buffer *buf;
	char pass[50];
		buf = bounded_buffer_new();
		assert(bounded_buffer_put(buf,"test1") == 0);
		assert(bounded_buffer_put(buf,"test2") == 0);
		assert(bounded_buffer_put(buf,"test3") == 0);
		assert(bounded_buffer_put(buf,"test4") == 0);
		assert(bounded_buffer_put(buf,"test5") == 0);
		assert(bounded_buffer_put(buf,"test6") == 0);
		assert(bounded_buffer_put(buf,"test7") == 0);
		assert(bounded_buffer_put(buf,"test8") == 0);
		assert(bounded_buffer_put(buf,"test9") == 0);
		assert(bounded_buffer_put(buf,"test10") == 0);
		assert(bounded_buffer_put(buf,"test11") == -1);
		
		bounded_buffer_get(buf, pass);
		assert(strcmp(pass,"test1")==0);
		
		assert(bounded_buffer_put(buf,"test11") == 0);
		
		bounded_buffer_get(buf,pass);
		assert(strcmp(pass,"test2")==0);

		bounded_buffer_get(buf,pass);
	    assert(strcmp(pass,"test3")==0);
		bounded_buffer_get(buf,pass);
		assert(strcmp(pass,"test4")==0);
		bounded_buffer_get(buf,pass);
		assert(strcmp(pass,"test5")==0);
		
		bounded_buffer_get(buf,pass);
        assert(strcmp(pass,"test6")==0);
        bounded_buffer_get(buf,pass);
        assert(strcmp(pass,"test7")==0);
        bounded_buffer_get(buf,pass);
        assert(strcmp(pass,"test8")==0);

		bounded_buffer_get(buf,pass);
        assert(strcmp(pass,"test9")==0);
        bounded_buffer_get(buf,pass);
        assert(strcmp(pass,"test10")==0);
        bounded_buffer_get(buf,pass);
        assert(strcmp(pass,"test11")==0);
		
		assert(bounded_buffer_get(buf,pass) == -1);

}


int main(){

	test_file_reader();
	test_buffer();
	printf("All tests succeeded\n");
	return EXIT_SUCCESS;
}

