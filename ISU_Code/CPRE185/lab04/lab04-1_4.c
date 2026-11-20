/*----------------------------------------------------------------------------
-		                 SE 185: Lab 04 - Debugging Code	    	         -
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
-	                                Notes                                    -
-----------------------------------------------------------------------------*/
// Compile with gcc lab04-1_4.c -o lab04-1_4
// Run with ./lab04-1_4
/* This program calculates the energy of one photon
 * of user-inputted wave-length of light */

/*----------------------------------------------------------------------------
-								Implementation								 -
-----------------------------------------------------------------------------*/
int main(int argc, char *argv[])
{
	//double speed_of_light!; had ! at the end witch was throwing up an error 
    double speed_of_light;
	//double wave-length; had - at the middle witch was throwing up an error 
    double wavelength;
	//double ~length_in_meters; had ~ at in the begining witch was throwing up an error 
    double length_in_meters;
	//double plank const; was declariing something while it was already declared and it needs to be a constant
    //double 0energy; exponent has no digits
	double energy;
	
	//plank const = 6.62606957 * pow(10, -34); // Planck's constant would not allow me to declare a variable while setting the variable equal to something 
    const double plank = 6.62606957 * pow(10, -34); // Planck's constant
	double plan;
	
	//speed_of_light! = 2.99792458 * pow(10, 8); had ! it expected a end statment before !
    speed_of_light = 2.99792458 * pow(10, 8); // Constant for the speed of light
	//wave-length = 0; had - it expected a end statment before -
    wavelength = 0;
    //~length_in_meters = 0;  had ~ it expected a end statment before ~
	length_in_meters = 0;
    //0energy = 0; exponent has no didget 
	energy = 0;

    printf("Welcome! This program will give the energy, in Joules,\n");
    printf("of 1 photon with a certain wave-length.\n");
    printf("Please input a wave-length of light in nano-meters.\n");
    printf("Please do not enter a negative, or zero, wave-length.\n");

	//scanf("%lf", &wave-length);
    scanf("%lf", &wavelength);
	
	//if (wave-length > 0.0)
    if (wavelength > 0.0)
    {	// ~length_in_meters = wave-length / pow(10, 9); had unexpected operators 
        length_in_meters = wavelength / pow(10, 9); // Converting nano-meters to meters 
		//0energy = (plank const * speed_of_light!) / ~length_in_meters; had unexpected operators
        energy = (plank * speed_of_light) / length_in_meters; // Calculating the energy of 1 photon
		//  printf("A photon with a wave-length of %08.3lf nano-meters, carries "
               //"\napproximately %030.25lf joules of energy.", wave-length, energy);  had unexpected operators 
        printf("A photon with a wave-length of %08.3lf nano-meters, carries "
               "\napproximately %030.25lf joules of energy.", wavelength, energy);
    } else
    {
        printf("Sorry, you put in an invalid number.");
        printf("Please rerun the program and try again.");
    }

    return 0;
}
