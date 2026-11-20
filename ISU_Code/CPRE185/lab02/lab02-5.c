/*----------------------------------------------------------------------------
-		        SE 185: Lab 02 - Solving Simple Problems in C	    	 	 -
-	Name: Mikhail McDade																	 -
- 	Section: 1																 -
-	NetID: mamcdade																     -
-	Date: 1/31/2023																	 -
-----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------
-								Includes									 -
-----------------------------------------------------------------------------*/
#include <stdio.h>
#include <math.h>   // Google this header file to learn more! :)

/*----------------------------------------------------------------------------
-								Implementation								 -
-----------------------------------------------------------------------------*/
int main(int argc, char *argv[])
{
    double a, b, c;
    double filler;
	
	printf("\nEnter a value for a: \n");
    scanf("%lf", &a);
	printf("\nEnter a value for b: \n");
	scanf("%lf", &b);
	
    filler = sqrt(pow(a,2)+pow(b,2));
	printf("\nThe value of c is %lf\n", filler);

    return 0;
}
