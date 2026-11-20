/*---------------------------------------------------------------------------
-					  SE 185: Lab 07 - Pointers, Pointers and More Pointers           -
-	Name: Mikhail McDade                                                               -
- 	Section: 1                                                                  -
-	NetID: mamcdade                                                                   -
-	Date: 3/21/2023                                                                  -
-                                                                           -
-   This file provides the outline for your program.                        -
----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------
-								Includes									                                   -
-----------------------------------------------------------------------------*/
#include <stdio.h>
#include <stdlib.h>

/* add whatever you see fit and need */

/*----------------------------------------------------------------------------
-	                            Prototypes                                   -
-----------------------------------------------------------------------------*/

/* put your function prototypes here */

/*----------------------------------------------------------------------------
-	                                Notes                                    -
-----------------------------------------------------------------------------*/

// compile with gcc -o lab07-1 lab07-1.c

/*-----------------------------------------------------------------------------
-								Implementation
-----------------------------------------------------------------------------*/
int main(int argc, char *argv[])
{

	//char inputName[64];
	//char inputMajor[64];
	char *name;
	char *major;
	
	name = (char*)malloc(1024);
	major = (char*)malloc(1024); 
	
	printf("\nEnter your Name: \n");
	scanf("%s", name);
	printf("\nEnter your Major: \n");
	scanf("%s", major);
	
	printf("\nYour Name and major are: %s , %s\n", name, major);
	
    return 0;

}

/*-----------------------------------------------------------------------------
-								Functions
-----------------------------------------------------------------------------*/

/* put your functions here */
