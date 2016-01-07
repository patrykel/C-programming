#include <stdio.h>
#include <stdlib.h>
#include <errno.h>


void report_error_in_function(char *function_name){
	printf("Error occured while calling: %s\nErrno = %d\n", function_name, errno);
	exit(-1);
}

int main(int argc, char** argv){
	if(argc != 3){
		printf("Wrong arguments number.\nExpected call format: %s filename_1 filename_2\n", argv[0]);
		return -1;
	}

	char *fname_r, *fname_w;
	fname_r = argv[1];
	fname_w = argv[2];

	printf("You want to read from %s and write to %s\n", fname_r, fname_w);

	FILE *frp, *fwp;
	if( (frp = fopen(fname_r, "r")) == NULL)
		report_error_in_function("fopen(fname_r, \"r\")");
	if( (fwp = fopen(fname_w, "w")) == NULL)
		report_error_in_function("fopen(fname_w, \"w\")");

	char c;
	int size = 1;
	int nmemb = 1;
	while(fread(&c,size,nmemb,frp) == nmemb){		//If an error occurs, or the end of the file is reached, 
													//the return value is a short item count (or zero).
		if(fwrite(&c, size, nmemb, fwp) != nmemb){
			report_error_in_function("fwrite(c,size,nmemb,fwp)");
			exit(-1);
		}
	}

	//check if error occured during fread
	if(ferror(frp) != 0)
		report_error_in_function("fread(c,size,nmemb,frp)");


	printf("Process finished with success!\n");
	return 0;
}