#include <stdio.h>
#include <stdlib.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

void report_error(char *msg){
	perror(msg);
	exit(-1);
}

int main(int argc, char** argv){
	if(argc != 3){
		printf("Wrong arguments number.\nExpected call format: %s filename_1 filename_2\n", argv[0]);
		return -1;
	}

	char *fpath_r = argv[1];
	char *fpath_w = argv[2];

	int fd_r = -1;
	int fd_w = -1;

	if( (fd_r = open(fpath_r, O_RDONLY)) == -1)
		report_error("In function: open(fpath_r, O_RDONLY)\n");
	if( (fd_w = open(fpath_w, O_WRONLY | O_CREAT, S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP)) == -1)
		report_error("In function: open(fpath_w, O_WRONLY | O_CREAT, S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP)\n");

	char c = -1;
	int read_return = -1;
	while( (read_return = read(fd_r, &c, 1)) != 0 ){
		if(read_return == -1)
			report_error("In function: read(fd_r, &c, 1)\n");

		if( write(fd_w, &c, 1) == -1)
			report_error("In function: write(fd_w, &c, 1)\n");
	}

	close(fd_r);
	close(fd_w);

	exit(0);
}    
