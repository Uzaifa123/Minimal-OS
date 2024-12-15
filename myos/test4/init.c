#include <stdio.h>   // For printf
#include <unistd.h>  // For sleep

int main() {

    printf("\033[H\033[J");
    
    while (1) { // Infinite loop
        printf("Hello, World!\n");
        sleep(1); // Delay for 1 second
    }
    return 0; // This will never execute
}
