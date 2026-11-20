#include "apc.h"

/* ---------- validation ---------- */

// Checks that a string is an optional sign followed by one or more digits only
static int is_valid_number(char *str)
{
    // i is the index where the digits should start
    int i = 0;

    // Skip a leading '+' or '-' sign
    if (str[0] == '-' || str[0] == '+')
        i = 1;

    // Reject empty strings and strings that contain only a sign
    if (str[i] == '\0')
        return FAILURE;

    // Check every remaining character
    while (str[i] != '\0')
    {
        // Reject anything that is not a digit from 0 to 9
        if (str[i] < '0' || str[i] > '9')
        {
            return FAILURE;
        }
        i++;
    }
    // Every character was a digit, so the number is valid
    return SUCCESS;
}

// Prints how to run the program, with one example for each operator
static void print_usage(char *prog)
{
    printf(CYAN "---------------------------------------------------\n" RESET);
    // prog is the program name (argv[0]), so the message matches however it was started
    printf(YELLOW "Usage:   %s <operand1> <operator> <operand2>\n", prog);
    printf("Example: %s 10 + 10\n", prog);
    printf("Example: %s 10 - 10\n", prog);
    printf("Example: %s 10 x 10\n", prog);
    printf("Example: %s 10 / 10\n" RESET, prog);
    printf(CYAN "---------------------------------------------------\n" RESET);
}

// Checks the argument count, both operands and the operator, returns SUCCESS or FAILURE
int cla_validation(int argc, char *argv[])
{
    // Expect exactly 4 arguments: program name, operand 1, operator, operand 2
    if (argc != 4)
    {
        printf(CYAN "---------------------------------------------------\n" RESET);
        printf(RED "ERROR! Invalid number of arguments.\n" RESET);
        print_usage(argv[0]);
        return FAILURE;
    }

    // The first operand must be a valid number
    if (is_valid_number(argv[1]) == FAILURE)
    {
        printf(CYAN "---------------------------------------------------\n" RESET);
        printf(RED "ERROR: 1st operand is not a valid number\n" RESET);
        printf(CYAN "---------------------------------------------------\n" RESET);
        return FAILURE;
    }

    // The second operand must be a valid number
    if (is_valid_number(argv[3]) == FAILURE)
    {
        printf(CYAN "---------------------------------------------------\n" RESET);
        printf(RED "ERROR: 2nd operand is not a valid number\n" RESET);
        printf(CYAN "---------------------------------------------------\n" RESET);
        return FAILURE;
    }

    // The operator must be a single character and one of + - x X /
    if (strlen(argv[2]) != 1 || strchr("+-xX/", argv[2][0]) == NULL)
    {
        printf(CYAN "---------------------------------------------------\n" RESET);
        printf(RED "ERROR: Invalid operator Type.\n" RESET);
        print_usage(argv[0]);
        return FAILURE;
    }

    // All checks passed
    return SUCCESS;
}

/* ---------- list creation ---------- */

// Builds a linked list from a string of digits, one node per digit, left to right
void create_list(char *opr, node **head, node **tail)
{
    // Loop until the end of the string ('\0' is false)
    for (int i = 0; opr[i]; i++)
    {
        // Convert the character to its numeric value ('7' - '0' = 7) and add it at the end
        insert_last(head, tail, opr[i] - '0');
    }
}

/* ---------- insertion ---------- */

// Adds a new digit at the front of the list (used to build results from right to left)
int insert_first(node **head, node **tail, int data)
{
    // Allocate memory for the new node
    node *new = malloc(sizeof(node));
    // Report failure if there is no memory left
    if (new == NULL)
        return FAILURE;

    // The new node becomes the first node, so it has no previous node
    new->data = data;
    new->prev = NULL;
    // Its next node is the old first node
    new->next = *head;

    // Empty list: the new node is both the head and the tail
    if (*head == NULL)
    {
        *tail = new;
    }
    // Non-empty list: link the old head back to the new node
    else
    {
        (*head)->prev = new;
    }
    // The new node is now the head
    *head = new;
    return SUCCESS;
}

// Adds a new digit at the end of the list (used to build lists from left to right)
int insert_last(node **head, node **tail, int data)
{
    // Allocate memory for the new node
    node *new = malloc(sizeof(node));
    // Report failure if there is no memory left
    if (new == NULL)
        return FAILURE;

    // The new node becomes the last node, so it has no next node
    new->data = data;
    new->next = NULL;
    // Its previous node is the old last node
    new->prev = *tail;

    // Empty list: the new node is the head
    if (*head == NULL)
    {
        *head = new;
    }
    // Non-empty list: link the old tail forward to the new node
    else
    {
        (*tail)->next = new;
    }
    // The new node is now the tail
    *tail = new;
    return SUCCESS;
}

/* ---------- delete / print ---------- */

// Frees every node in the list and resets the tail pointer
int delete_list(node **head, node **tail)
{
    // Nothing to delete if the list is already empty
    if (*head == NULL)
    {
        return FAILURE;
    }

    node *temp;
    // Walk through the list from head to tail
    while (*head)
    {
        // Save the current node, move the head forward, then free the saved node
        temp = *head;
        *head = (*head)->next;
        free(temp);
    }
    // The list is empty now, so the tail must not point to freed memory
    *tail = NULL;
    return SUCCESS;
}

// Prints the digits of the list from head to tail
void print_list(node *head)
{
    // Print one digit per node until the end of the list
    while (head)
    {
        printf("%d", head->data);
        head = head->next;
    }
}

/* ---------- helpers for the operations ---------- */

// Counts how many digits (nodes) are in the list
int list_len(node *head)
{
    int len = 0;
    // Add 1 for every node until the end of the list
    while (head)
    {
        len++;
        head = head->next;
    }
    return len;
}

// Removes leading zeros but keeps one node if the number is 0
void remove_pre_zeros(node **head)
{
    // Keep going while the first digit is 0 and there is at least one more digit after it
    while (*head && (*head)->data == 0 && (*head)->next != NULL)
    {
        // Save the zero node, move the head forward and cut the link back to the old head
        node *temp = *head;
        *head = temp->next;
        (*head)->prev = NULL;
        // Free the removed zero node
        free(temp);
    }
}

// Compares two numbers and returns OPERAND1, OPERAND2 or SAME (leading zeros must already be removed)
int compare_list(node *head1, node *head2)
{
    // The number with more digits is larger
    int len1 = list_len(head1);
    int len2 = list_len(head2);

    if (len1 > len2)
        return OPERAND1;
    if (len2 > len1)
        return OPERAND2;

    // Same length: compare digit by digit starting from the most significant digit
    while (head1 && head2)
    {
        // The first digit that differs decides which number is larger
        if (head1->data > head2->data)
            return OPERAND1;
        if (head2->data > head1->data)
            return OPERAND2;
        head1 = head1->next;
        head2 = head2->next;
    }
    // Every digit matched, so the numbers are equal
    return SAME;
}