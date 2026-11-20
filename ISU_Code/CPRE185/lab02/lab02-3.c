/*----------------------------------------------------------------------------
-		        SE 185: Lab 02 - Solving Simple Problems in C	    	 	 -
-	Name: Mikhail McDade																 -
- 	Section: 1																 -
-	NetID: mamcdade																     -
-	Date: 1/31/2023																	 -
-----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------
-								Includes									 -
-----------------------------------------------------------------------------*/
#include <stdio.h>

/*----------------------------------------------------------------------------
-								Implementation								 -
-----------------------------------------------------------------------------*/
int main(int argc, char *argv[])
{
    int integer_result;
    double decimal_result;

    integer_result = 77 / 5;
    printf("The value of 77/5 is %d, using integer math.\n", integer_result); // I changed it form %lf to %d 

    integer_result = 2 + 3;
    printf("The value of 2+3 is %d.\n", integer_result);// I included ,integer_result after the ""  

    decimal_result = 1.0 / 22.0;
    printf("The value 1.0/22.0 is %lf.\n", decimal_result); // I changed the %d to %lf 

    return 0;
}
