#include "apc.h"

void addition(node *tail1, node *tail2, node **headR, node **tailR)
{
	// Initialize carry and sum variables
	int carry = 0, sum;

	// Continue until both lists are traversed and no carry remains
	while (tail1 != NULL || tail2 != NULL || carry != 0)
	{
		sum = carry; // Initialize sum with the previous carry

		// Check whether the first list has remaining nodes
		if (tail1 != NULL)
		{
			sum += tail1->data;	 // Add the current digit of the first list
			tail1 = tail1->prev; // Move to the previous node in the first list
		}

		// Check whether the second list has remaining nodes
		if (tail2 != NULL)
		{
			sum += tail2->data;	 // Add the current digit of the second list
			tail2 = tail2->prev; // Move to the previous node in the second list
		}

		// Insert the unit digit at the beginning of the result list
		insert_first(headR, tailR, sum % 10);

		// Calculate carry for the next addition
		carry = sum / 10;
	}
}