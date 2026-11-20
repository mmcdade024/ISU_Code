/*----------------------------------------------------------------------------
-		                    SE 185: Lab 06 - Bop-It!	    	             -
-	Name: Mikhail McDade																 -
- 	Section: 1																 -
-	NetID: mamcdade																     -
-	Date: 3/7/2023																	 -
-----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------
-								Includes									 -
-----------------------------------------------------------------------------*/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <sys/time.h>
/*----------------------------------------------------------------------------
-	                            Prototypes                                     -
-----------------------------------------------------------------------------*/
char UserIn(char targetChar, double totalTime, double t, double start_t, int temp2);

/*----------------------------------------------------------------------------
-	                            Notes                                        -
-----------------------------------------------------------------------------*/
// Compile with gcc lab06.c -o lab06
// Run with ./ds4rd.exe -d 054c:05c4 -D DS4_BT -t -b | ./lab06

/*----------------------------------------------------------------------------
-								Implementation								 -
-----------------------------------------------------------------------------*/

long long timeInMilliseconds(void) {
    struct timeval tv;

    gettimeofday(&tv,NULL);
    return (((long long)tv.tv_sec)*1000)+(tv.tv_usec/1000);
}

int main(int argc, char *argv[])
{
	double t;
	double start_t;
	double totalTime = 8000;
	int temp = 0;
	int temp2 = 0;
	char userstart;
	char targetChar;
	char userInput;
	srand(time(NULL));
	
	printf("Please press any key to begin!");
	scanf(" %c", &userstart);
	printf("\n");

	start_t = timeInMilliseconds();
	
	while (1)
	{
		t = timeInMilliseconds();

		if ((targetChar != toupper(userInput)) || (totalTime < t - start_t)){
			if (targetChar != toupper(userInput)){
				printf("Wrong Key! :(\n");
				printf("You lose!\n");
				printf("You made it through %d rounds! \n", temp);
			}
	
			else if(totalTime < t - start_t){
				printf("You ran out of time. :(\n");
				printf("Thnaks for playing!\n");
				printf("You made it through %d rounds!\n", temp);
			}
			break;
		}
		
		targetChar = rand()%26 + 65;	
		userInput = UserIn(targetChar, totalTime, t, start_t, temp2);
		
		if ((targetChar == toupper(userInput)) && (totalTime > t - start_t)){
			temp ++;
		}
	}
	printf("\n---------------------------------------------------------\n");
	printf("Please press any key to begin! ");
	printf("\n");
	scanf(" %c", &userstart);
	
	for (int i = 0; i < 26; i++){
			printf("%c: %d\n", i + 65, i + 65);
	}
	printf("\n");
	
	start_t = timeInMilliseconds();
	targetChar = ' ';
	userInput = ' ';
	temp = 0;
	temp2 = 1;
	while (1)
	{
		t = timeInMilliseconds();
		
		if ((targetChar != toupper(userInput)) || (totalTime < t - start_t)){
			if (targetChar != toupper(userInput)){
				printf("Wrong Key! :(\n");
				printf("You lose!\n");
				printf("You made it through %d rounds!\n", temp);
			}
	
			else if(totalTime < t - start_t){
				printf("You ran out of time. :(\n");
				printf("Thnaks for playing!\n");
				printf("You made it through %d rounds!\n", temp);
			}
			break;
		}

		targetChar = rand()%26 + 65;
		userInput = UserIn(targetChar, totalTime, t, start_t, temp2);
		
		if ((targetChar == toupper(userInput)) && (totalTime > t - start_t)){
			temp ++;
		}
	}	
	return 0;
}

char UserIn(char targetChar, double totalTime, double t, double start_t, int temp2){
	char userInput;
	if (temp2 == 0){
		printf("Press the '%c' key\n", targetChar);
	}
	else
		printf("Press the '%d' key\n", targetChar);
	printf("You have %.0lf milliseconds to respond\n", totalTime - (t - start_t));
	scanf(" %c", &userInput);
	printf("\n");
	
	return userInput;
}