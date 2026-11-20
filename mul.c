#include "apc.h"

void multiplication(node *tail1, node *tail2, node **headR, node **tailR)
{
    if ((tail1->data == 0 && tail1->prev == NULL) || // Check whether the first number is zero
        (tail2->data == 0 && tail2->prev == NULL))   // Check whether the second number is zero
    {
        insert_first(headR, tailR, 0); // Insert zero into the result list
        return;                        // Stop multiplication if either number is zero
    }

    *headR = NULL; // Initialize the result list head to NULL
    *tailR = NULL; // Initialize the result list tail to NULL

    int pos = 0, i;      // Initialize position counter and loop variable
    node *temp2 = tail2; // Create a temporary pointer to traverse the second list

    // Traverse each digit of the second number from right to left
    while (temp2 != NULL)
    {
        // Store the current digit of the second number
        int digit = temp2->data;

        // Initialize a temporary list for the partial product
        node *headT = NULL, *tailT = NULL;

        // Repeat according to the current digit position
        for (i = 0; i < pos; i++)
        {
            // Append zeros to represent multiplication by powers of 10
            insert_first(&headT, &tailT, 0);
        }

        // Initialize carry and product variables
        int carry = 0, product;
        node *temp1 = tail1; // Create a temporary pointer to traverse the first list

        // Traverse each digit of the first number from right to left
        while (temp1 != NULL)
        {
            product = temp1->data * digit + carry;      // Multiply the current digits and add the carry
            insert_first(&headT, &tailT, product % 10); // Insert the unit digit into the partial product
            carry = product / 10;                       // Calculate the carry for the next multiplication
            temp1 = temp1->prev;                        // Move to the previous digit of the first number
        }

        // Check whether any carry remains after multiplication
        if (carry > 0)
        {
            // Insert the remaining carry into the partial product
            insert_first(&headT, &tailT, carry);
        }

        // Initialize a new list to store the addition result
        node *headnew = NULL, *tailnew = NULL;

        // Callind addition function to add the current partial product to the previous result
        addition(*tailR, tailT, &headnew, &tailnew);

        delete_list(headR, tailR);   // Delete the previous result list to free its memory
        delete_list(&headT, &tailT); // Delete the temporary partial product list

        *headR = headnew; // Update the result head with the newly calculated result
        *tailR = tailnew; // Update the result tail with the newly calculated result

        // Increase the position to shift the next partial product by one place
        pos = pos + 1;
        temp2 = temp2->prev; // Move to the previous digit of the second number

        // Remove unnecessary leading zeros from the result
        remove_pre_zeros(headR);
    }
}