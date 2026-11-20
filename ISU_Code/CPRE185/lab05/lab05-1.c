/*----------------------------------------------------------------------------
-		        SE 185: Lab 02 - Solving Simple Problems in C	    	 	 -
-	Name: Mikhail McDade																	 -
- 	Section: 1																 -
-	NetID: mamcdade														     -
-	Date: 2/21/2023																	 -
-----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------
-								Includes									 -
-----------------------------------------------------------------------------*/
#include <stdio.h>

double isPON(double num);
/*----------------------------------------------------------------------------
-								Implementation								 -
-----------------------------------------------------------------------------*/
double isPON(double num){
	if (num > 0){
		printf("\n%lf is a postive number\n", num);
	}else{
		printf("\n%lf is a negative number\n", num);
	}
}

int main(int argc, char *argv[])
{
    double ui1, ui2;
	char pon;
	
	printf("Enter num 1: \n");
	scanf("%lf", &ui1);
	
	printf("Enter num 2: \n");
	scanf("%lf", &ui2);
	
	isPON(ui1);
	isPON(ui2);
	
    return 0;
}

