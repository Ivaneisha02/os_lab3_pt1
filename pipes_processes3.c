#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <sys/wait.h>

/**
 * Executes the command "cat scores | grep Lakers".  In this quick-and-dirty
 * implementation the parent doesn't wait for the child to finish and
 * so the command prompt may reappear before the child terminates.
 *
 */

int main(int argc, char **argv){
    int pipefd1[2];
    int pipefd2[2];
    int pid;
    int pid2;
    int pid3;
    int status;

    char *cat_args[] = {"cat", "scores", NULL};
    char *grep_args[] = {"grep", "Toronto", NULL};
    char *sort_args[] = {"sort", NULL, NULL};
    // make a pipe (fds go in pipefd[0] and pipefd[1])

    grep_args[1] = argv[1];
    if (argc <1) {
        printf("Please enter a grep argument:\n");
        exit(1);
    }
    pipe(pipefd1);
    pipe(pipefd2);
    pid = fork();

    if (pid == 0)
    {
        // child gets here and handles "grep Villanova"

        // replace standard input with input part of pipe
        dup2(pipefd1[1], 1);

        // close unused hald of pipe

        close(pipefd1[1]);
        close(pipefd1[0]);
        close(pipefd2[0]);
        close(pipefd2[1]);

        // execute grep

        //execvp("grep", grep_args);
        execvp(*cat_args,cat_args);
    }
    else if (pid < 0){
        perror("fork error");
    }
    else
    {
      // parent gets here and handles "cat scores" P1

      // replace standard output with output part of pipe
        pid2 = fork();
        if (pid2 ==0 ){
            dup2(pipefd1[0], 0);
            dup2(pipefd2[1], 1);

            close(pipefd1[1]);
            close(pipefd1[0]);
            close(pipefd2[0]);
            close(pipefd2[1]);

            execvp(*grep_args, grep_args);
        }
        else if (pid2 < 0) {
            printf("grep fork error\n");
        }
        else {
            pid3 = fork();
            if (pid3 ==0 ){
                dup2(pipefd2[0],0);
                close(pipefd1[1]);
                close(pipefd1[0]);
                close(pipefd2[0]);
                close(pipefd2[1]);

                execvp(*sort_args, sort_args);
            }
            else if (pid3 < 0) {
                perror("sort fork error\n");
            }
        }
    }
    close(pipefd1[1]);
    close(pipefd1[0]);
    close(pipefd2[1]);
    close(pipefd1[1]);

    int i;
    for (i=0; i<3; i++) {
        wait(&status);
    }

    // close unused unput half of pipe

    //close(pipefd[0]);

    // execute cat

    //execvp("cat", cat_args);
    }
