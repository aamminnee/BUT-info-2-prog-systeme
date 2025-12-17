#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <signal.h>

#define BUF_SIZE 256

volatile double current_avg = 0.0;

void print_avg(int sig) {
    printf("Moyenne courante : %.2f\n", current_avg);
    alarm(5);
}

int main() {
    int pipe_c2p[2]; 
    int pipe_p2c[2]; 
    pid_t pid;

    if (pipe(pipe_c2p) == -1 || pipe(pipe_p2c) == -1) {
        perror("pipe");
        exit(1);
    }

    pid = fork();
    if (pid == -1) {
        perror("fork");
        exit(1);
    }

    if (pid == 0) {
        close(pipe_c2p[0]);
        close(pipe_p2c[1]);

        signal(SIGALRM, print_avg);
        alarm(5);

        char buffer[BUF_SIZE];
        ssize_t n;
        
        while ((n = read(STDIN_FILENO, buffer, BUF_SIZE - 1)) > 0) {
            buffer[n] = '\0';
            int val = atoi(buffer);
            
            write(pipe_c2p[1], &val, sizeof(int));
            read(pipe_p2c[0], (void*)&current_avg, sizeof(double));
        }
        
        close(pipe_c2p[1]);
        close(pipe_p2c[0]);
        exit(0);
    } else {
        close(pipe_c2p[1]);
        close(pipe_p2c[0]);
        
        int val;
        long sum = 0;
        long count = 0;
        double avg;

        while (read(pipe_c2p[0], &val, sizeof(int)) > 0) {
            sum += val;
            count++;
            avg = (double)sum / count;
            write(pipe_p2c[1], &avg, sizeof(double));
        }
        
        close(pipe_c2p[0]);
        close(pipe_p2c[1]);
        wait(NULL);
    }
    return 0;
}