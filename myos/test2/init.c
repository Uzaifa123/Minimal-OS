#include <syscall.h>
#include <fcntl.h>

// Define NULL manually since we can't use stddef.h
#define NULL ((void*)0)

// Function prototypes
void print_string(char *str);
void delay(int ticks);
unsigned long _syscall(int num, void *a0, void *a1, void *a2, void *a3, void *a4, void *a5);
unsigned long _strlen(char *sz);

// Function to execute the shell (either busybox or sh)
void exec_shell(char *shell_path) {
    // Arguments for execve system call
    char *args[] = {shell_path, NULL}; // Command and arguments

    // Execute the shell using the execve syscall (number 59)
    _syscall(SYS_execve, shell_path, args, 0, 0, 0, 0);
}

// Main function
int main() {
    char *msg = "MyOS 0.0.0.1 Initializing...\n";

    // Initialize and print the message
    print_string(msg);
    delay(1000000000);


    // Path to the BusyBox or sh executable (adjust as needed)
    char *shell_path = "/bin/sh";  // You can also try "/bin/busybox"

    // Run the shell
    exec_shell(shell_path);

   

    return 0;
}

// Implementation of print_string (you can use your _syscall function)
void print_string(char *str) {
    _syscall(SYS_write, (void *)1 /*stdout*/, str, (void *)_strlen(str), 0, 0, 0);
}

// Implementation of delay function
void delay(int ticks) {
    for (int i = 0; i < ticks; i++) {
        // Do nothing, just delay (busy wait)
    }
}

// Implementation of _strlen
unsigned long _strlen(char *sz) {
    unsigned long count = 0;
    while (*sz++) {
        count++;
    }
    return count;
}
