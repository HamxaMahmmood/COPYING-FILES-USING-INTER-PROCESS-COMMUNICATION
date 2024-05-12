#include <stdio.h>

#define BUFFER_SIZE 1024

char* buffer1;
char* buffer2;

void printresult(){
    printf("Received message 2 from Python: %s\n", buffer2);
    printf("Received message 1 from Python: %s\n", buffer1);
    }
int main() {

    
    // Read from stdin (which is redirected from the first pipe)
    fgets(buffer1, BUFFER_SIZE, stdin);
    // Read from file descriptor 3 (which is redirected from the second pipe)
    fgets(buffer2, BUFFER_SIZE, fdopen(3, "r"));
    printresult();
    // Print the received messages
    
    return 0;
}