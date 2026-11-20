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
double operation(double num1, double num2, double UserI);

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

double operation(double num1, double num2, double UserI){
	double output;
	if(UserI == 1){
		output = num1 + num2;
		printf("%lf + %lf = %lf",num1, num2, output);
	}
	else if(UserI == 2){
		output = num1 - num2;
		printf("%lf - %lf = %lf",num1, num2, output);
	}
	else if(UserI == 3){
		output = num1 / num2;
		printf("%lf / %lf = %lf",num1, num2, output);
	}
	else if(UserI == 4){
		output = num1 * num2;
		printf("%lf * %lf = %lf",num1, num2, output);
	}else{
		printf("Number out of range");
	}
}

int main(int argc, char *argv[])
{
    double ui1, ui2, Type;
	char YoN;
	
	printf("Enter num 1: \n");
	scanf("%lf", &ui1);
	
	printf("Enter num 2: \n");
	scanf("%lf", &ui2);
	
	isPON(ui1);
	isPON(ui2);
	
	printf("\nDo you wish to make a calculation ");
	scanf("%s", &YoN);
	
	if ((YoN == 'y') || (YoN == 'Y')){
		printf("\nType 1 for Addition\nType 2 for Subtraction\nType 3 for Division\nType 4 for Multiplication\n --> ");
		scanf("%lf", &Type);
		operation(ui1, ui2, Type);

	}else{
		printf("\nGoodbye!!!\n");
	}
	
    return 0;
}
