#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>
#include <sys/wait.h>
#include <pthread.h>

#define KILOBYTE 1024


void *sending_thread(void *filename);
void *recieving_thread(void *filename);

char *videofile1;
char *videofile2;
char *videofile4 = "./folder2/recieved2.pdf";
int main(void)
{

    char buffer1[KILOBYTE];
    char buffer2[KILOBYTE];

    // Read from stdin (which is redirected from the first pipe)
    fgets(videofile1, KILOBYTE, stdin);
    // Read from file descriptor 3 (which is redirected from the second pipe)
    fgets(videofile2, KILOBYTE, fdopen(3, "r"));

    // Print the received messages
    printf("Received message 1 from Python: %s\n", buffer1);
    printf("Received message 2 from Python: %s\n", buffer2);
    return 0;
    // CREATING PIPES TO SEND/RECIEVE VIDEO FILES
    int pipe1[2];
    int pipe2[2];
    pipe(pipe1);
    pipe(pipe2);

    // CREATING TWO BUFFERS FOR READING AND WRITING 1 KB DATA FROM THE RECIEVED AND SENT FILE SIMULTANEOUSLY

    // FORKING A CHILD PROCESS
    pid_t pid = fork();

    // CHILD PROCESS RESPONSIBLE FOR SENDING DATA TO THE PARENT/RECIEVER PROCESS
    if (pid == 0)
    {
        char write_buffer[KILOBYTE];

        // CREATING THE SENDING THREAD
        pthread_t thread3;
        pthread_create(&thread3, NULL, *sending_thread, &pipe2[1]);

        close(pipe1[0]);
        close(pipe2[0]);
        int fd1 = open(videofile1, O_RDONLY); // OPENING VIDEO FILE TO BE SEND
        lseek(fd1, 0, SEEK_SET);

        ssize_t byte_read;
        while ((byte_read = read(fd1, write_buffer, KILOBYTE)) > 0) // READING 1 KB DATA FROM THE FILE TO BE SEND
        {
            write(pipe1[1], write_buffer, KILOBYTE); // SENDING 1 KB DATA THROUGH THE PIPE
        }
        close(pipe1[1]); // CLOSING THE WRITE END OF THE PIPE
        close(fd1);
        pthread_join(thread3, NULL);
        exit(EXIT_SUCCESS);
    }
    else if (pid > 0) // RECIEVER PARENT PROCESS RESPONSIBLE FOR RECIEVING 1 KB DATA FROM THE PIPE AND WRITING TO A NEWLY CREATED VIDEO FILE
    {
        char read_buffer[KILOBYTE];

        char *videofile3 = "./folder2/recieved1.pdf";
        close(pipe1[1]);
        close(pipe2[1]);
        // CREATING THE RECIEVING THREAD
        pthread_t thread4;
        pthread_create(&thread4, NULL, *recieving_thread, &pipe2[0]);

        int fd2 = open(videofile2, O_WRONLY | O_CREAT | O_TRUNC, 0644); // CREATING AND OPENING A NEW VIDEO FILE

        ssize_t byte_read;

        while ((byte_read = read(pipe1[0], read_buffer, KILOBYTE)) > 0) // READING 1 KB DATA FROM THE READ END OF THE PIPE
        {
            write(fd2, read_buffer, byte_read); // WRITING 1KB DATA TO THE NEWLY CREATED VIDEO FILE
        }
        close(pipe1[0]); // CLOSING READ END OF THE PIPE
        close(fd2);
        pthread_join(thread4, NULL);
        wait(NULL);
        printf("Files sent/recieved successfully.\n");
    }

    else if (pid < 0)

    {
        printf("Forking of child process failed!!!!");
    }

    return 0;
}
// SENDING THREAD OF CHILD PROCESS RESPONSIBLE FOR SENDING THE VIDEO FILE TO THE RECIEVING THREAD OF PARENT PROCESS
void *sending_thread(void *pipe)
{
    int *pipe2 = (int *)pipe;
    char write_buffer[KILOBYTE];

    char *videofile2 = "./folder1/file2.pdf";
    int fd1 = open(videofile2, O_RDONLY); // OPENING VIDEO FILE TO BE SEND
    lseek(fd1, 0, SEEK_SET);

    ssize_t byte_read;
    while ((byte_read = read(fd1, write_buffer, KILOBYTE)) > 0) // READING 1 KB DATA FROM THE FILE TO BE SEND
    {
        write(*pipe2, write_buffer, KILOBYTE); // SENDING 1 KB DATA THROUGH THE PIPE
    }
    close(*pipe2); // CLOSING THE WRITE END OF THE PIPE
    close(fd1);
    pthread_exit(NULL);
}
// RECIEVING THREAD OF PARENT PROCESS RESPONSIBLE FOR RECIEVING THE VIDEO FILE FROM THE SENDING THREAD OF CHILD PROCESS
void *recieving_thread(void *pipe)
{
    int *pipe2 = (int *)pipe;

    char read_buffer[KILOBYTE];


    int fd2 = open(videofile2, O_WRONLY | O_CREAT | O_TRUNC, 0644); // CREATING AND OPENING A NEW VIDEO FILE

    ssize_t byte_read;

    while ((byte_read = read(*pipe2, read_buffer, KILOBYTE)) > 0) // READING 1 KB DATA FROM THE READ END OF THE PIPE
    {
        write(fd2, read_buffer, byte_read); // WRITING 1KB DATA TO THE NEWLY CREATED VIDEO FILE
    }
    close(*pipe2); // CLOSING READ END OF THE PIPE
    close(fd2);
    pthread_exit(NULL);
}
