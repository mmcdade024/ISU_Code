/*---------------------------------------------------------------------------
-					  SE 185: Lab 07 - Pointers, Pointers and More Pointers           -
-	Name: Mikhail McDade                                                               -
- 	Section: 1                                                                  -
-	NetID: mamcdade                                                                   -
-	Date: 3/21/2023                                                                    -
-                                                                           -
-   This file provides the outline for your program.                        -
----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------
-								Includes									                                   -
-----------------------------------------------------------------------------*/
#include <stdio.h>
#include <stdlib.h>

/*----------------------------------------------------------------------------
-	                            Prototypes                                   -
-----------------------------------------------------------------------------*/

void makeChange(int remMoney, int *dollar, int *quarter, int *nickel, int *pennies);

/*----------------------------------------------------------------------------
-	                                Notes                                    -
-----------------------------------------------------------------------------*/

// compile with gcc -o lab07-1 lab07-1.c

/*-----------------------------------------------------------------------------
-								Implementation
-----------------------------------------------------------------------------*/
int main(int argc, char *argv[])
{

	int priceProduct;
	int moneyPaid;
	int remMoney;
	
	int dollar;
	int quarter;
	int nickel;
	int pennies;
	

	
	printf("\nEnter the price of the product in pennies: \n");
	scanf("%d", &priceProduct);
	printf("\nEnter the amount of money payed in pennies: \n");
	scanf("%d", &moneyPaid);
	
	remMoney = moneyPaid - priceProduct;
	
	makeChange(remMoney, &dollar, &quarter, &nickel, &pennies);
	printf("You have %d dollar, %d quarter, %d nickel, and %d pennies left\n", dollar, quarter, nickel, pennies);

    return 0;

}

/*-----------------------------------------------------------------------------
-								Functions
-----------------------------------------------------------------------------*/

void makeChange(int remMoney, int *dollar, int *quarter, int *nickel, int *pennies){
	
	*dollar = remMoney / 100;
	remMoney -= (*dollar * 100);
	*quarter = remMoney / 25; 
	remMoney -= (*quarter * 25);
	*nickel = remMoney / 10;
	remMoney -= (*nickel * 10);
	*pennies = remMoney;
	
}