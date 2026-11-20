/*
Documentation

Name        : Varun M M
Student id  : 26018_042
Batch id    : 26018D
Start Date  :
End date    :

Description: The following main functions are implemented in the Arbitrary Precision Calculator(APC) project to perform arithmetic operations on large numbers
beyond the standard data type limits.

main()              :-> Handles command line arguments and selects the required arithmetic operation such as addition, subtraction, multiplication or division.
					 -> Validates the input operands and operator, identifies the signs of operands and calls the appropriate arithmetic function.
					 -> Displays the final result and handles the operation status.

addition()          :-> Performs addition of two large numbers using linked lists.
					 -> Adds corresponding digits from right to left while handling carry values.

subtraction()       :-> Performs subtraction of two large numbers using linked lists.
					 -> Subtracts corresponding digits while handling borrow values and maintains the correct sign of the result.

multiplication()    :-> Performs multiplication of two large numbers using linked lists.
					 -> Multiplies individual digits and handles carry values to generate the final product.

division()          :-> Performs division of two large numbers using the long division method.
					 -> Calculates the quotient and remainder while handling division by zero.

insert_first()      :-> Inserts a new digit at the beginning of the linked list.
					 -> Used to construct the result by storing digits in the required order.

insert_last()       :-> Inserts a new digit at the end of the linked list.
					 -> Used to build linked lists containing input numbers and intermediate results.

delete_list()       :-> Deletes all nodes from the linked list.
					 -> Frees the dynamically allocated memory used by the linked list.

print_list()        :-> Traverses and displays the digits stored in the linked list.

compare_list()      :-> Compares two linked lists representing numbers.
					 -> Determines whether the first number is greater than, smaller than or equal to the second number.

remove_pre_zeros()  :-> Removes unnecessary leading zeros from a linked list.
					 -> Maintains the correct representation of the number.

cla_validation()    :-> Validates the command line arguments.
					 -> Checks the number of arguments and verifies whether the given arithmetic operator is valid.

*/

/*
Sample input:

./a.out 12345678901234567890 + 98765432109876543210
-----------------------------------------------------
		   Arbitrary Precision Calculator
-----------------------------------------------------
Operand 1: 12345678901234567890
Operation: Addition
Operand 2: 98765432109876543210
-----------------------------------------------------
Result:    111111111011111111100
-----------------------------------------------------
			   Calculation Done
-----------------------------------------------------
*/
#include "apc.h"

int main(int argc, char *argv[])
{
	// Head and tail pointers for operand 1, operand 2, result and remainder lists
	node *head1 = NULL, *tail1 = NULL;
	node *head2 = NULL, *tail2 = NULL;
	node *headR = NULL, *tailR = NULL;
	node *headRem = NULL, *tailRem = NULL;

	// Stop with exit code 1 if the arguments, operands or operator are invalid
	if (cla_validation(argc, argv) == FAILURE)
	{
		return 1;
	}

	// Signs start positive (1); resSign will hold the sign of the final result
	int sign1 = 1, sign2 = 1, resSign = 1;
	// op1 and op2 point to the operand strings so we can skip their sign characters
	char *op1 = argv[1], *op2 = argv[3];

	// Operand 1 is negative: remember the sign and skip the '-' character
	if (op1[0] == '-')
	{
		sign1 = -1;
		op1++;
	}
	// Operand 1 has an explicit '+': just skip it, the sign stays positive
	else if (op1[0] == '+')
	{
		op1++;
	}

	// Operand 2 is negative: remember the sign and skip the '-' character
	if (op2[0] == '-')
	{
		sign2 = -1;
		op2++;
	}
	// Operand 2 has an explicit '+': just skip it, the sign stays positive
	else if (op2[0] == '+')
	{
		op2++;
	}

	// Create 2 lists of operands (digits only, no sign)
	create_list(op1, &head1, &tail1);
	create_list(op2, &head2, &tail2);

	// Remove leading zeros so numbers like 007 become 7
	remove_pre_zeros(&head1);
	remove_pre_zeros(&head2);

	// Operator character taken from the 2nd command line argument
	char oper = argv[2][0];
	// Becomes 0 if the operator turns out to be invalid, so nothing is printed
	int valid = 1;

	// Print the header banner
	printf(CYAN "-----------------------------------------------------\n" RESET);
	printf(GREEN "	   Arbitrary Precision Calculator\n" RESET);
	printf(CYAN "-----------------------------------------------------\n" RESET);

	// Print operand 1, adding '-' only if it is negative and not zero (avoids "-0")
	printf(BLUE "Operand 1: " RESET);
	if (sign1 == -1 && !(head1->data == 0 && head1->next == NULL))
		printf("-");
	print_list(head1);
	printf("\n");

	// Print the name of the operation being performed
	printf(BLUE "Operation: " RESET);
	switch (oper)
	{
	case '+':
		printf("Addition\n");
		break;
	case '-':
		printf("Subtraction\n");
		break;
	// Both 'x' and 'X' mean multiplication
	case 'x':
	case 'X':
		printf("Multiplication\n");
		break;
	case '/':
		printf("Division\n");
		break;
	}

	// Print operand 2, adding '-' only if it is negative and not zero (avoids "-0")
	printf(BLUE "Operand 2: " RESET);
	if (sign2 == -1 && !(head2->data == 0 && head2->next == NULL))
		printf("-");
	print_list(head2);
	printf("\n");

	// Choose the calculation based on the operator
	switch (oper)
	{
	// Addition and subtraction share one block because a - b is the same as a + (-b)
	case '+':
	case '-':
		// For subtraction, flip operand 2's sign so we can treat it as an addition
		if (oper == '-')
			sign2 = -sign2;

		// Same signs: add the magnitudes and keep that common sign
		if (sign1 == sign2)
		{
			addition(tail1, tail2, &headR, &tailR);
			resSign = sign1;
		}
		// Different signs and operand 2 is bigger: subtract smaller from bigger, result takes operand 2's sign
		else if (compare_list(head1, head2) == OPERAND2)
		{
			subtraction(tail2, tail1, &headR, &tailR);
			resSign = sign2;
		}
		// Different signs and operand 1 is bigger or equal: result takes operand 1's sign
		else
		{
			subtraction(tail1, tail2, &headR, &tailR);
			resSign = sign1;
		}
		break;

	// Multiplication: 'x' and 'X' are both accepted
	case 'x':
	case 'X':
		multiplication(tail1, tail2, &headR, &tailR);
		// Same signs give a positive product, different signs give a negative one
		resSign = (sign1 == sign2) ? 1 : -1;
		break;

	// Division: produces both a quotient (headR) and a remainder (headRem)
	case '/':
		division(head1, head2, &headR, &tailR, &headRem, &tailRem);
		// Quotient is positive for same signs, negative for different signs
		resSign = (sign1 == sign2) ? 1 : -1;
		break;

	// Safety net for an unknown operator (validation normally catches this earlier)
	default:
		printf(RED "Invalid operator\n" RESET);
		valid = 0;
	}

	// Print only if the operator was valid and a result list was created (not on divide by zero)
	if (valid && headR != NULL)
	{
		// Clean up any leading zeros left in the result
		remove_pre_zeros(&headR);

		// Division prints a quotient and a remainder
		if (oper == '/')
		{
			printf(CYAN "-----------------------------------------------------\n" RESET);
			printf(GREEN "Quotient : " RESET);
			// Show '-' only for a negative quotient that is not zero
			if (resSign == -1 && !(headR->data == 0 && headR->next == NULL))
				printf("-");
			print_list(headR);
			printf("\n");

			// The remainder always takes the sign of the dividend (operand 1)
			printf(GREEN "Remainder: " RESET);
			if (sign1 == -1 && !(headRem->data == 0 && headRem->next == NULL))
				printf("-");
			print_list(headRem);
			printf("\n");
			printf(CYAN "-----------------------------------------------------\n" RESET);
			printf(GREEN "		Calculation Done\n" RESET);
			printf(CYAN "-----------------------------------------------------\n" RESET);
		}
		// Addition, subtraction and multiplication print a single result
		else
		{
			printf(CYAN "-----------------------------------------------------\n" RESET);
			printf(GREEN "Result:	  " RESET);
			// Show '-' only for a negative result that is not zero
			if (resSign == -1 && !(headR->data == 0 && headR->next == NULL))
				printf("-");
			print_list(headR);
			printf("\n");
			printf(CYAN "-----------------------------------------------------\n" RESET);
			printf(GREEN "		Calculation Done\n" RESET);
			printf(CYAN "-----------------------------------------------------\n" RESET);
		}
	}

	// Free all lists to avoid memory leaks
	delete_list(&head1, &tail1);
	delete_list(&head2, &tail2);
	delete_list(&headR, &tailR);
	delete_list(&headRem, &tailRem);

	// Exit code 0 tells the shell the program finished successfully
	return 0;
}