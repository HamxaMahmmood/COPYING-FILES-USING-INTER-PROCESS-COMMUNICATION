#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>
#include <sys/wait.h>
#include <pthread.h>
#include <string.h>

#define KILOBYTE 1024

void *sending_thread(void *filename);
void *recieving_thread(void *filename);
struct ThreadArgs
{
    char *result;
    int *pipe;
};

int main(void)
{


    char temp1[KILOBYTE];
    char temp2[KILOBYTE];

    char videofile1[KILOBYTE];
    char videofile2[KILOBYTE];

    // Read from stdin (which is redirected from the first pipe)
    fgets(videofile1, KILOBYTE, stdin);
    // Read from file descriptor 3 (which is redirected from the second pipe)
    fgets(videofile2, KILOBYTE, fdopen(3, "r"));
  // Read from file descriptor 4 (which is redirected from the folder1 pipe)
    fgets(temp1, KILOBYTE, fdopen(4, "r"));
    // Read from file descriptor 5 (which is redirected from the folder2 pipe)
    fgets(temp2, KILOBYTE, fdopen(5, "r"));

    // videofile1
    size_t length1 = strlen(temp1) + strlen(videofile1) + 1;
    char result1[length1];
    strcpy(result1, temp1);
    strcat(result1, videofile1);

    // videofile2
    size_t length2 = strlen(temp1) + strlen(videofile2) + 1;
    char result2[length2];
    strcpy(result2, temp1);
    strcat(result2, videofile2);
    // videofile3
    size_t length3 = strlen(temp2) + strlen(videofile1) + 1;
    char result3[length3];
    strcpy(result3, temp2);
    strcat(result3, videofile1);
    // videofile4
    size_t length4 = strlen(temp2) + strlen(videofile2) + 1;
    char result4[length4];
    strcpy(result4, temp2);
    strcat(result4, videofile2);

    // Print the received messages
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
        struct ThreadArgs args = {result2, &pipe2[1]};

        pthread_t thread3;
        pthread_create(&thread3, NULL, sending_thread, &args);

        close(pipe1[0]);
        close(pipe2[0]);
        int fd1 = open(result1, O_RDONLY); // OPENING VIDEO FILE TO BE SEND
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

        close(pipe1[1]);
        close(pipe2[1]);
        // CREATING THE RECIEVING THREAD
        struct ThreadArgs args = {result4, &pipe2[0]};

        pthread_t thread4;
        pthread_create(&thread4, NULL, *recieving_thread, &args);

        int fd2 = open(result3, O_WRONLY | O_CREAT | O_TRUNC, 0644); // CREATING AND OPENING A NEW VIDEO FILE

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
void *sending_thread(void *arg)
{
    // Cast the argument back to the correct structure type
    struct ThreadArgs *args = (struct ThreadArgs *)arg;

    char *result3 = args->result;
    int *pipe2 = args->pipe;
    int fd1 = open(result3, O_RDONLY); // OPENING VIDEO FILE TO BE SEND
    lseek(fd1, 0, SEEK_SET);

    char write_buffer[KILOBYTE];
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
void *recieving_thread(void *arg)
{
    struct ThreadArgs *args = (struct ThreadArgs *)arg;

    char *result4 = args->result;
    int *pipe2 = args->pipe;

    char read_buffer[KILOBYTE];

    int fd2 = open(result4, O_WRONLY | O_CREAT | O_TRUNC, 0644); // CREATING AND OPENING A NEW VIDEO FILE

    ssize_t byte_read;

    while ((byte_read = read(*pipe2, read_buffer, KILOBYTE)) > 0) // READING 1 KB DATA FROM THE READ END OF THE PIPE
    {
        write(fd2, read_buffer, byte_read); // WRITING 1KB DATA TO THE NEWLY CREATED VIDEO FILE
    }
    close(*pipe2); // CLOSING READ END OF THE PIPE
    close(fd2);
    pthread_exit(NULL);
}
