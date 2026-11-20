// Header guard: prevents this file from being included more than once
#ifndef APC_H
#define APC_H

// Standard library headers used across the project
#include <stdio.h>  // printf for printing output
#include <string.h> // strlen and strchr for validating the operator
#include <stdlib.h> // malloc and free for linked list nodes

// ANSI escape codes for coloring terminal output
#define RED "\033[31m"     // used for error messages
#define GREEN "\033[32m"   // used for the title, results and "Calculation Done"
#define YELLOW "\033[33m"  // used for the usage message
#define BLUE "\033[34m"    // used for the "Operand" and "Operation" labels
#define MAGENTA "\033[35m" // defined but not currently used
#define CYAN "\033[36m"    // used for the separator lines
#define WHITE "\033[37m"   // defined but not currently used
#define RESET "\033[0m"    // returns the terminal to its normal color

// Return values used by the helper functions
#define SUCCESS 1 // the function worked
#define FAILURE 0 // the function failed

// Return values of compare_list() to say which number is larger
#define SAME 0     // both numbers are equal
#define OPERAND1 1 // the first number is larger
#define OPERAND2 2 // the second number is larger

// One digit of a number, stored in a doubly linked list (most significant digit at the head)
typedef struct node
{
    struct node *prev; // pointer to the previous (more significant) digit, NULL at the head
    int data;          // a single digit from 0 to 9
    struct node *next; // pointer to the next (less significant) digit, NULL at the tail
} node;

// Adds two numbers digit by digit from the tail (rightmost digit), result goes to headR/tailR
void addition(node *tail1, node *tail2, node **headR, node **tailR);

// Subtracts the second number from the first (the first must be larger or equal), result goes to headR/tailR
void subtraction(node *tail1, node *tail2, node **headR, node **tailR);

// Multiplies two numbers using partial products added together, result goes to headR/tailR
void multiplication(node *tail1, node *tail2, node **headR, node **tailR);

// Long division: quotient goes to headR/tailR and remainder goes to headRem/tailRem
void division(node *head1, node *head2, node **headR, node **tailR, node **headRem, node **tailRem);

// Checks the argument count, both operands and the operator, returns SUCCESS or FAILURE
int cla_validation(int argc, char *argv[]);

// Builds a linked list from a string of digits, one node per digit
void create_list(char *opr, node **head, node **tail);

// Adds a new digit at the front of the list (used to build results from right to left)
int insert_first(node **head, node **tail, int data);

// Adds a new digit at the end of the list (used to build lists from left to right)
int insert_last(node **head, node **tail, int data);

// Frees every node in the list and resets the tail pointer
int delete_list(node **head, node **tail);

// Prints the digits of the list from head to tail
void print_list(node *head);

// Compares two numbers and returns SAME, OPERAND1 or OPERAND2 (leading zeros must be removed first)
int compare_list(node *head1, node *head2);

// Returns the number of digits (nodes) in the list
int list_len(node *head);

// Removes leading zeros from the list but keeps one node if the number is zero
void remove_pre_zeros(node **head);

#endif