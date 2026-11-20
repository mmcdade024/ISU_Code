/*----------------------------------------------------------------------------
-		        SE 185: Lab 02 - Solving Simple Problems in C	    	 	 -
-	Name: Mikhail McDade																	 -
- 	Section: 1																 -
-	NetID: mamcdade														     -
-	Date: 2/21/2023																		 -
-----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------
-								Includes									 -
-----------------------------------------------------------------------------*/
#include <stdio.h>
#include <string.h>

char country(char region[25], char name[25]);

/*----------------------------------------------------------------------------
-								Implementation								 -
-----------------------------------------------------------------------------*/
char country(char region[25], char name[25]){
	char area[25];
	
	if (strcmp(region, "NA") == 0){
		printf("here is a list of countries based on your region\n-Unites States-\n-Canada-\n-Mexico-\n-other-\n--> ");
		scanf("%s", &area);
	}
	else if (strcmp(region, "EU") == 0){
		printf("here is a list of countries based on your region\n-Poland-\n-Bulgaia-\n-Slovakia-\n-other-\n--> ");
		scanf("%s", &area);
	}
	else if (strcmp(region, "SA") == 0){
		printf("here is a list of countries based on your region\n-Angola-\n-Botswana-\n-Lesotho-\n-other-\n--> ");
		scanf("%s", &area);
	}
	if (strcmp(area, "other") == 0){
		printf("What country is other\n--> ");
		scanf("%s", area);
	}
	printf("%s is from %s in %s", name, area, region);
}

int main(int argc, char *argv[])
{
	char region[25];
	char name[25];
	
	printf("What is your name\n--> ");
	scanf("%s", &name);
	printf("What region are you from?\n--> ");
	scanf("%s", &region);
	
	country(region, name);

    return 0;
}
