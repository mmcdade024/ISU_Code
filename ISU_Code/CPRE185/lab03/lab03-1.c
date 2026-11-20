/*----------------------------------------------------------------------------
-		    SE 185: Lab 03 - Introduction to the DS4 and Functions	    	 -
-	Name: Mikhail McDade																	 -
- 	Section: 1																 -
-	NetID: mamcdade																     -
-	Date: 2/7/2023																	 -
-----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------
-								Includes									 -
-----------------------------------------------------------------------------*/
#include <stdio.h>
#include <math.h>

/*----------------------------------------------------------------------------
-	                            Prototypes                                   -
-----------------------------------------------------------------------------*/
double magnitude(double x, double y, double z);

/*----------------------------------------------------------------------------
-	                                Notes                                    -
-----------------------------------------------------------------------------*/
// Compile with gcc lab03-1.c -o lab03-1
// Run with ./ds4rd.exe -d 054c:05c4 -D DS4_BT -t -a | ./lab03-1

/*----------------------------------------------------------------------------
-								Implementation								 -
-----------------------------------------------------------------------------*/
int milliseconds(int t){
	
	return t % 1000;
}
int seconds(int t){
	int milli;
	int sec;
	int min;
	int tempmilli;
	
	milli = t;
	tempmilli = milli;
	milli = milli % 1000;
	sec = tempmilli / 1000;
	min = sec / 60;
	sec = sec - (min * 60);
	
	return sec;
}
int minutes(int t){
	int milli;
	int sec;
	int min;
	int tempmilli;
	
	tempmilli = t;
	sec = tempmilli / 1000;
	min = sec / 60;
	
	return min;
}	

int main(int argc, char *argv[])
{
    /* DO NOT MODIFY THESE VARIABLE DECLARATIONS */
    int t;
    double ax, ay, az;

    while (1)
    {
        scanf("%d, %lf, %lf, %lf", &t, &ax, &ay, &az);

        /* CODE SECTION 0 */
        printf("\nEchoing output: %d, %8.3lf, %8.3lf, %8.3lf\n", t, ax, ay, az);


        /* 	CODE SECTION 1 */
        printf("\nAt %d ms, the acceleration's magnitude was: %7.3lf\n", t, magnitude(ax, ay, az));


        /* 	CODE SECTION 2 */

			
		printf("At %d minutes, %d seconds, and %d milliseconds it was: %lf\n",
		minutes(t), seconds(t), milliseconds(t), magnitude(ax, ay, az));

    }

    return 0;
}

/* Put your functions here */

/**
 * Calculates and returns the magnitude of three given values.
 *
 * @param x - The x-axis scanned values from the DS4 controller.
 * @param y - The y-axis scanned values from the DS4 controller.
 * @param z - The z-axis scanned values from the DS4 controller.
 * @return - The magnitude of the given values.
 */
double magnitude(double x, double y, double z)
{
    
    return sqrt(pow(x, 2) + pow(y, 2) + pow(z, 2));
}

