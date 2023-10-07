#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <sys/time.h>
#include <errno.h>
#define ARGVMAX 100

double elapsed_time() {
    struct timeval t1, t2;
    gettimeofday(&t1, NULL);
    wait(NULL);
    gettimeofday(&t2, NULL);
    double elapsed = (t2.tv_sec - t1.tv_sec) + (t2.tv_usec - t1.tv_usec) / 1000000.0;
    return elapsed;
}

char *name_of_file(int argc, char *argv[ARGVMAX]) {
    int size = strlen(argv[argc-1]);
    char *name_of_file = (char*) malloc(size);
    if (!name_of_file) {
        perror("malloc");
        exit(EXIT_FAILURE);
    }
    for(int i = 1, j = 0 ; i < size; i++) {
        name_of_file[j++] = argv[argc-1][i];
    }
    name_of_file[size-1] = '\0';
    return name_of_file;
}

void rumcmd(int argc, char *argv[ARGVMAX]) {
    
    pid_t pid = fork();
    switch(pid) {
        case -1:
            perror("fork");
            return;
        case 0:
            if(argv[argc-1][0] == '=') {
                char *file = name_of_file(argc, argv);
                close(1);
                if (creat(file, 0666) == -1) {
                    perror(file);
                    free(file);
                    exit(EXIT_FAILURE);
                }
                argv[argc-1] = NULL;
                free(file);
            }
            execvp(argv[1], &argv[1]);
            perror(argv[1]);
            exit(1);
        default:
            printf("\nElapsed time = %6f s\n", elapsed_time());
    }
}


int main(int argc,  char *argv[ARGVMAX]){
    if (argc == 2 && strcmp(argv[1], "exit") == 0) {
        printf("\nElapsed time = %6f s\n", elapsed_time());
        exit(EXIT_SUCCESS);
    }
    
    rumcmd(argc, argv);
    return 0;
}