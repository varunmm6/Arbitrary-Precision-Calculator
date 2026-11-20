#include "apc.h"

void subtraction(node *tail1, node *tail2, node **headR, node **tailR)
{
    // Initialize borrow and subtraction variables
    int borrow = 0, sub;

    // Continue until all digits of the first list are processed
    while (tail1 != NULL)
    {
        sub = tail1->data - borrow; // Subtract the previous borrow from the current digit
        borrow = 0;                 // Reset borrow before performing the next subtraction

        // Check whether the second list has remaining digits
        if (tail2 != NULL)
        {
            sub -= tail2->data;  // Subtract the current digit of the second list
            tail2 = tail2->prev; // Move to the previous node in the second list
        }

        // Check whether borrowing is required
        if (sub < 0)
        {
            sub += 10;  // Add 10 to make the result digit positive
            borrow = 1; // Set borrow for the next digit
        }

        // Insert the calculated digit at the beginning of the result list
        insert_first(headR, tailR, sub);

        // Move to the previous node in the first list
        tail1 = tail1->prev;
    }

    // Remove leading zeros from the result list
    remove_pre_zeros(headR);
}