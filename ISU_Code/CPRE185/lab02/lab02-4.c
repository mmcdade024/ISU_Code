/*----------------------------------------------------------------------------
-		        SE 185: Lab 02 - Solving Simple Problems in C	    	 	 -
-	Name: Mikhail McDade															 -
- 	Section: 1																 -
-	NetID: mamcdade																     -
-	Date: 1/31/2023																	 -
-----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------
-								Includes									 -
-----------------------------------------------------------------------------*/
#include <stdio.h>
#include <math.h>

/*----------------------------------------------------------------------------
-								Implementation								 -
-----------------------------------------------------------------------------*/
int main(int argc, char *argv[])
{
	int integer_result;
    double decimal_result, pie, area, conversion, temp;
	
	integer_result = 6427 + 1725;
    printf("The value of 6427 + 1725 is %d", integer_result);
	
	integer_result = (6971 * 1725) - 95;
    printf("\nThe value of (6971 * 1725) - 95 is %d", integer_result);
	
	decimal_result = 79 + 12 / 5;
    printf("\nThe value 79 + 12 / 5 is %lf.", decimal_result);
	
	decimal_result = 3640.0 / 107.9;
    printf("\nThe value 3640.0 / 107.9 is %lf.", decimal_result);
	
	integer_result = (22 / 3) * 3;
    printf("\nThe value of (22 / 3) * 3 is %d", integer_result);
	
	integer_result = 22 / (3 * 3);
    printf("\nThe value of 22 / (3 * 3) is %d", integer_result);
	
	decimal_result = 22 / (3 * 3);
    printf("\nThe value 22 / (3 * 3) is %lf.", decimal_result);
	
	decimal_result = 22 / 3* 3;
    printf("\nThe value 22 / 3* 3 is %lf.", decimal_result);
	
	decimal_result = (22.0 / 3) * 3.0;
    printf("\nThe value (22.0 / 3) * 3.0 is %lf.", decimal_result);
	
	integer_result = 22.0 / (3 * 3.0);
    printf("\nThe value of 22.0 / (3 * 3.0) is %d", integer_result);
	
	decimal_result = 22.0 / 3.0 * 3.0;
    printf("\nThe value 22.0 / 3.0 * 3.0 is %lf.", decimal_result);
	
	pie = atan(1) * (4);
	area = pow(23.567,2) / (4 * pie);
	printf("\nwith a circumference of 23.567 the area of the circle is %lf", area);
	
	conversion = 14 * 0.3048;
	printf("\n14 feet to meters is %lf", conversion);
	
	temp = (76 - 32)/1.8;
	printf("\n76 degrees Fahrenheit to Celsius is %lf", temp);
	
    return 0;
}
