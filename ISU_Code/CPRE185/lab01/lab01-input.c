/*----------------------------------------------------------------------------
-		    SE 185: Lab 01 - Getting to Know the 185 Environment		 	 -
-	Name: Mikhail McDade																 -
- 	Section: 1															 -
-	NetID: mamcdade															     -
-	Date: 1/24/2023																	 -
-----------------------------------------------------------------------------*/

/*
	Here is a block comment.
	These lines don't run when you compile or run the code.
	I.e., they're the machine doesn't see them, only you.
*/

/*----------------------------------------------------------------------------
-								Includes									 -
-----------------------------------------------------------------------------*/
#include <stdio.h>
#include <stdlib.h>
/*----------------------------------------------------------------------------
-								Implementation								 -
-----------------------------------------------------------------------------*/
int main(int argc, char *argv[])
{
	char net_id[1000];
	
	int number = 0;
	
	printf("\nValue before input: %d\n", number);
	
	printf("\nType a number: ");
	scanf("%d", &number);
	
	printf("\nType a NetID: ");
	scanf("%s", &net_id);
	
    printf("\nYour input was %d ", number);
    printf("\and Your ISU email is %s@iastate.edu", net_id);

    return 0;
}