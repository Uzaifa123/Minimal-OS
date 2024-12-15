#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_INPUT_SIZE 100

// Function to perform the basic calculation (addition, subtraction, multiplication, division)
double evaluate(char* expression) {
    double num1, num2;
    char operator;

    // Use sscanf to parse the expression
    if (sscanf(expression, "%lf %c %lf", &num1, &operator, &num2) != 3) {
        printf("Invalid expression!\n");
        return 0;
    }

    // Perform the operation based on the operator
    switch (operator) {
        case '+':
            return num1 + num2;
        case '-':
            return num1 - num2;
        case '*':
            return num1 * num2;
        case '/':
            if (num2 != 0)
                return num1 / num2;
            else {
                printf("Error: Division by zero!\n");
                return 0;
            }
        default:
            printf("Invalid operator! Supported operators are +, -, *, /.\n");
            return 0;
    }
}

int main() {
    char input[MAX_INPUT_SIZE];
    printf("Welcome to Simple Shell Calculator!\n");

    while (1) {
        printf("calc> ");  // Display the prompt
        fgets(input, MAX_INPUT_SIZE, stdin);  // Get user input
        input[strcspn(input, "\n")] = 0;  // Remove the trailing newline character
        
        if (strcmp(input, "exit") == 0) {  // Exit the shell if the user types "exit"
            printf("Exiting calculator shell.\n");
            break;
        }

        // Call the evaluate function to calculate the result of the expression
        double result = evaluate(input);
        
        // Display the result if it's valid
        if (result != 0) {
            printf("Result: %.2f\n", result);
        }
    }

    return 0;
}
