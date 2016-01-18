#include <stropts.h>
#include <poll.h>

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

void report_error_and_terminate(char *msg){
    perror(msg);
    exit(-1);
}

int main(int argc, char **argv){
    if(argc != 3){
        printf("Wrong arguments number.\nExpected call format: %s filename_1 filename_2\n", argv[0]);
        return -1;
    }

    char *fpath_r = argv[1];
    char *fpath_w = argv[2];

    struct pollfd fds[2];    
    if( (fds[0].fd = open(fpath_r, O_RDONLY)) == -1)
        report_error_and_terminate("open");
    if( (fds[1].fd = open(fpath_w, O_WRONLY | O_CREAT, S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP)) == -1)
        report_error_and_terminate("open");
    fds[0].events = POLLIN;
    fds[1].events = POLLOUT;
    int timeout_msecs = 500;
    int nfds = 2;


    char c=-1;
    bool eof_detected = false;
    bool data_read = false;
    int poll_ret, read_ret, write_ret;
    poll_ret = read_ret = write_ret = -1;

    while(!eof_detected){

        poll_ret = poll(fds, nfds, timeout_msecs);

        if(poll_ret < 0)
            report_error_and_terminate("poll");
        else if(poll_ret == 0)
            printf("No file descriptor have been selected.\n");
        else{
            /* An event on one of the fds has occurred. */
            int i=0;
            for (i=0; i<2; i++) {
                if ( (fds[i].revents & POLLIN) && !data_read) {
                    read_ret = read(fds[i].fd, &c, 1);
                    
                    if(read_ret == -1){
                        report_error_and_terminate("read");
                    }
                    else if(read_ret == 0){
                        eof_detected = true;   
                    }
                    else{
                        data_read = true;
                    }
                    
                }
                if ( (fds[i].revents & POLLOUT) && data_read ) {                    
                    int write_ret = -1;
                    write_ret = write(fds[i].fd, &c, 1);
                    if(write_ret == -1)
                        report_error_and_terminate("read");

                    data_read = false;
                }
            }
        }
    }

    exit(0);
}

