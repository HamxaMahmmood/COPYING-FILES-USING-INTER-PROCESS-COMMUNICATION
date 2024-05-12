#include <stdio.h>
#include <string.h>

#define KILOBYTE 1024

char *buffer1;
char *buffer2;
int main()
{
    char buffer1[KILOBYTE];
    char buffer2[KILOBYTE];

    char *temp1 = "./folder1/";
    char *temp2 = "./folder2/";

    char videofile1[KILOBYTE];
    char videofile2[KILOBYTE];

    // Read from stdin (which is redirected from the first pipe)
    fgets(videofile1, KILOBYTE, stdin);
    // Read from file descriptor 3 (which is redirected from the second pipe)
    fgets(videofile2, KILOBYTE, fdopen(3, "r"));
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
    printf("files created %s %s %s %s \n", result1, result2, result3, result4);
    return 0;
}
