#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>
#include <sys/wait.h>

#define KILOBYTE 1024

int main(void)
{
    char temp1[KILOBYTE];
    char temp2[KILOBYTE];

    char videofile1[KILOBYTE];

    // Read from stdin (which is redirected from the first pipe)
    fgets(temp1, KILOBYTE, stdin);
    // Read from file descriptor 3 (which is redirected from the second pipe)
    fgets(temp2, KILOBYTE, fdopen(3, "r"));
    // Read from file descriptor 4 (which is redirected from the folder1 pipe)
    fgets(videofile1, KILOBYTE, fdopen(4, "r"));
    // Read from file descriptor 5 (which is redirected from the folder2 pipe)

    // videofile1
    size_t length1 = strlen(temp1) + strlen(videofile1) + 1;
    char result1[length1];
    strcpy(result1, temp1);
    strcat(result1, videofile1);

    // videofile2
    size_t length2 = strlen(temp2) + strlen(videofile1) + 1;
    char result2[length2];
    strcpy(result2, temp2);
    strcat(result2, videofile1);
    int fd1;
    int fd2;

    // CREATING PIPE TO SEND/RECIEVE VIDEO FILE
    int pipefd[2];
    pipe(pipefd);

    // CREATING TWO BUFFERS FOR READING AND WRITING 1 KB DATA FROM THE RECIEVED AND SENT FILE SIMULTANEOUSLY
    char read_buffer[KILOBYTE];
    char write_buffer[KILOBYTE];

    // FORKING A CHILD PROCESS
    pid_t pid = fork();

    // CHILD PROCESS RESPONSIBLE FOR SENDING DATA TO THE PARENT/RECIEVER PROCESS
    if (pid == 0)
    {
        close(pipefd[0]);
        fd1 = open(result1, O_RDONLY); // OPENING VIDEO FILE TO BE SEND
        lseek(fd1, 0, SEEK_SET);

        ssize_t byte_read;
        while ((byte_read = read(fd1, write_buffer, KILOBYTE)) > 0) // READING 1 KB DATA FROM THE FILE TO BE SEND
        {
            write(pipefd[1], write_buffer, KILOBYTE); // SENDING 1 KB DATA THROUGH THE PIPE
        }
        close(pipefd[1]); // CLOSING THE WRITE END OF THE PIPE
        close(fd1);
        exit(EXIT_SUCCESS);
    }
    else if (pid > 0) // RECIEVER PARENT PROCESS RESPONSIBLE FOR RECIEVING 1 KB DATA FROM THE PIPE AND WRITING TO A NEWLY CREATED VIDEO FILE
    {
        double kilobytes_recieved = 0;
        close(pipefd[1]);

        fd2 = open(result2, O_WRONLY | O_CREAT | O_TRUNC, 0644); // CREATING AND OPENING A NEW VIDEO FILE

        ssize_t byte_read;

        while ((byte_read = read(pipefd[0], read_buffer, KILOBYTE)) > 0) // READING 1 KB DATA FROM THE READ END OF THE PIPE
        {
            write(fd2, read_buffer, byte_read); // WRITING 1KB DATA TO THE NEWLY CREATED VIDEO FILE
        }
        close(pipefd[0]); // CLOSING READ END OF THE PIPE
        close(fd2);
        printf("File sent/recieved successfully.\n");
    }

    else if (pid < 0)

    {
        printf("Forking of child process failed!!!!");
    }

    return 0;
}
