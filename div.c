#include "apc.h"

void division(node *head1, node *head2, node **headR, node **tailR, node **headRem, node **tailRem)
{
    // Checking whether the divisor is zero
    if (head2->data == 0 && head2->next == NULL)
    {
        printf(CYAN "-----------------------------------------------------\n" RESET);
        printf(RED "ERROR: Divide by zero is not possible.\n" RESET);
        printf(CYAN "-----------------------------------------------------\n" RESET);
        return; // Stop division if the divisor is zero
    }

    // Initialize a temporary pointer to find the tail of the divisor
    node *tail2 = head2;

    // Traverse until the last node of the divisor
    while (tail2->next != NULL)
    {
        tail2 = tail2->next; // Move to the next node
    }

    node *headrem = NULL; // Initialize the remainder list head
    node *tailrem = NULL; // Initialize the remainder list tail

    *headR = NULL; // Initialize the quotient list head
    *tailR = NULL; // Initialize the quotient list tail

    // Create a temporary pointer to traverse the dividend
    node *temp = head1;

    // Traverse each digit of the dividend from left to right
    while (temp != NULL)
    {
        // Append the current dividend digit to the remainder
        insert_last(&headrem, &tailrem, temp->data);
        remove_pre_zeros(&headrem); // Remove unnecessary leading zeros from the remainder

        // Initialize the quotient digit counter to zero
        int count = 0;

        // Continue subtraction while the remainder is greater than or equal to the divisor
        while (compare_list(headrem, head2) != OPERAND2)
        {
            // Initialize a new list to store the subtraction result
            node *headnew = NULL, *tailnew = NULL;

            // Calling subtraction function to Subtract the divisor from the current remainder
            subtraction(tailrem, tail2, &headnew, &tailnew);

            delete_list(&headrem, &tailrem); // Delete the previous remainder list
            headrem = headnew;               // Update the remainder head with the subtraction result
            tailrem = tailnew;               // Update the remainder tail with the subtraction result
            count++;                         // Increment the quotient digit after each successful subtraction
        }

        // Append the calculated quotient digit to the quotient list
        insert_last(headR, tailR, count);

        // Move to the next digit of the dividend
        temp = temp->next;
    }

    // Remove leading zeros from the final quotient
    remove_pre_zeros(headR);

    *headRem = headrem; // Store the final remainder head
    *tailRem = tailrem; // Store the final remainder tail
}