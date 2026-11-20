#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <sys/time.h>

typedef struct userInfo{
	char name[3];
	int score;
	char tempName[3];
	int tempScore;
}userInfo;

long long timeInMilliseconds(void) { 
    struct timeval tv;

    gettimeofday(&tv,NULL);
    return (((long long)tv.tv_sec)*1000)+(tv.tv_usec/1000);
}

void ScoreBoardPrint();
void ScoreBoardWrite(char* userName, int newScore, int *gone);

int main(int argc, char* argv[]) {
	
	char start[99];
	char userName[4];
	
	int symbNum[99];
	int userHistory[99];
	int userIn;
	int CharinInt;
	int gameOver = 0;
	int turnWent = 1;
	int turnComplete = 0;
	int score = 0;
	int* gone = 0;
	int timeTotal = 700;
	int timeI;
	int timeF;
	
	ScoreBoardPrint();

	srand(time(NULL)); //<-- used to set up the random number generator 
	printf("\n----------------------------------------------------------------------------------------------------------------------------------------------------------------------\n");
	printf("RULES: Symbols will appear on the console and one will be circled at a time. Then the user will have to enter the order in which the symbols were circled.           |");
	printf("\n----------------------------------------------------------------------------------------------------------------------------------------------------------------------\n");
	while(1){
		int nameLen = 0;
		int b = 0;
		int num = 0;
		printf("\nEnter your first 3 initails: ");
		scanf("%s", userName);
		nameLen = strlen(userName);
		for (int i = 0; i < nameLen; i++){
			num = userName[i];
			if (!((num <= 90 && num >= 65) || (num >= 97 && num <= 122))){  //<-- checks to see if the user has enterd something that is not a character using ascii #
				b++;
			}
		}
		if(nameLen == 3 && b == 0){
			break;
		}
		else{
			printf("\n-------------------------\n");
			printf("INVALID INPUT TRY AGAIN |");
			printf("\n-------------------------\n");
		}
	}
	scanf("%*[^\n]"); //<-- placed here to compansate for line 285 if not here code gives an error 

	printf("\n\nAre you ready to start the game?(type something then hit enter): ");
	scanf("%s", &start[99]);
	scanf("%*[^\n]"); //<-- placed here to compansate for line 285 if not here code gives an error 
	
	while (gameOver == 0){
		
		for(int i = 0; i < turnWent; i++){ 
			symbNum[i] = rand()%4;
			symbNum[i] += 1; //<-- +1ed because the random number would generate 0 - 3 which I didnt need to do but I figured it would be nicer looking to print latter on and it makes it easier to compare the users inputs with the computers
			timeI = timeInMilliseconds();
			while (1){
			
				timeF = timeInMilliseconds();

				if ((symbNum[i] == 1) && (timeTotal < timeF - timeI)){ //<-- checks to see if the correct amount of time has passed
					printf("\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n"); //<-- used to space out the shapes so it appears like it disappeared 
					printf("\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n");
					printf("\n-------------------------------------------------\n\n");
					printf(" _________________ \n");
					printf("|1.)          _   |\n");
					printf("|	     | |  |\n");
					printf("|         ___| |  |\n");
					printf("|        (___} |  |\n"); 
					printf("|        (___} |  |\n"); 
					printf("|        (___} |  |\n");   
					printf("|         ``````  |\n");
					printf(" ``````````````````\n");
					printf("\n");
					printf(" 2.)      _____    \n");
					printf("         | ^ ^ |   \n");  
					printf("         |o   o|   \n");  
					printf("         | ooo |   \n");
					printf("          ``````   \n");
					printf("\n");
					printf(" 3.)     _______   \n");
					printf("        |  ___  |  \n");
					printf("        | |   | |  \n");
					printf("        | |  o| |  \n");
					printf("        | |   | |  \n");
					printf("        `````````  \n");
					printf("\n");
					printf(" 4.)     ______    \n");
					printf("        /     /    \n");
					printf("        \\     \\  \n");
					printf("        /     /    \n");
					printf("        \\     \\  \n"); 
					printf("         ``````    \n");
					printf("\n-------------------------------------------------\n");
					break;
				}
				else if ((symbNum[i] == 2) && (timeTotal < timeF - timeI)){
					printf("\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n");
					printf("\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n");
					printf("\n-------------------------------------------------\n\n");
					printf(" 1.)          _    \n");
					printf("	     | | \n");
					printf("          ___| |   \n");
					printf("         (___} |   \n"); 
					printf("         (___} |   \n");  
					printf("         (___} |   \n");   
					printf("          ``````   \n");
					printf("\n");
					printf(" _________________ \n");
					printf("|2.)      _____   |\n");
					printf("|        | ^ ^ |  |\n");  
					printf("|        |o   o|  |\n");  
					printf("|        | ooo |  |\n");
					printf("|         ``````  |\n");
					printf("|                 |\n");
					printf("|                 |\n");
					printf(" ``````````````````\n");
					printf("\n");
					printf(" 3.)     _______   \n");
					printf("        |  ___  |  \n");
					printf("        | |   | |  \n");
					printf("        | |  o| |  \n");
					printf("        | |   | |  \n");
					printf("        `````````  \n");
					printf("\n");
					printf(" 4.)     ______    \n");
					printf("        /     /    \n");
					printf("        \\     \\  \n");
					printf("        /     /    \n");
					printf("        \\     \\  \n");
					printf("         ``````    \n");
					printf("\n-------------------------------------------------\n");
					break;
				}
				else if ((symbNum[i] == 3) && (timeTotal < timeF - timeI)){
					printf("\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n");
					printf("\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n");
					printf("\n-------------------------------------------------\n\n");
					printf(" 1.)          _    \n");
					printf("	     | | \n");
					printf("          ___| |   \n");
					printf("         (___} |   \n"); 
					printf("         (___} |   \n");  
					printf("         (___} |   \n");   
					printf("          ``````   \n");
					printf("\n");
					printf(" 2.)      _____    \n");
					printf("         | ^ ^ |   \n");  
					printf("         |o   o|   \n");  
					printf("         | ooo |   \n");
					printf("          ``````   \n");
					printf("\n");
					printf(" _________________ \n");
					printf("|3.)     _______  |\n");
					printf("|       |  ___  | |\n");
					printf("|       | |   | | |\n");
					printf("|       | |  o| | |\n");
					printf("|       | |   | | |\n");
					printf("|       ````````` |\n");
					printf("|                 |\n");
					printf(" ``````````````````\n");
					printf("\n");
					printf(" 4.)     ______    \n");
					printf("        /     /    \n");
					printf("        \\     \\  \n");
					printf("        /     /    \n");
					printf("        \\     \\  \n");
					printf("         ``````    \n");
					printf("\n-------------------------------------------------\n");
					break;
				}
				else if ((symbNum[i] == 4) && (timeTotal < timeF - timeI)){
					printf("\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n");
					printf("\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n");
					printf("\n-------------------------------------------------\n\n");
					printf(" 1.)          _    \n");
					printf("	     | | \n");
					printf("          ___| |   \n");
					printf("         (___} |   \n"); 
					printf("         (___} |   \n");  
					printf("         (___} |   \n");   
					printf("          ``````   \n");
					printf("\n");
					printf(" 2.)      _____    \n");
					printf("         | ^ ^ |   \n");  
					printf("         |o   o|   \n");  
					printf("         | ooo |   \n");
					printf("          ``````   \n");
					printf("\n");
					printf(" 3.)     _______   \n");
					printf("        |  ___  |  \n");
					printf("        | |   | |  \n");
					printf("        | |  o| |  \n");
					printf("        | |   | |  \n");
					printf("        `````````  \n");
					printf("\n");
					printf(" _________________ \n");
					printf("|4.)     ______   |\n");
					printf("|       /     /   |\n");
					printf("|       \\     \\   |\n");
					printf("|       /     /   |\n");
					printf("|       \\     \\   |\n");
					printf("|        ``````   |\n");
					printf("|                 |\n");
					printf(" ``````````````````\n");
					printf("\n-------------------------------------------------\n");
					break;
				}
			}
			timeI = timeInMilliseconds();
			while(1){
				
				timeF = timeInMilliseconds();
				if(timeTotal < timeF - timeI){
					printf("\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n");
					printf("\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n");
					printf("\n-------------------------------------------------\n\n");
					printf(" 1.)          _    \n");
					printf("	     | | \n");
					printf("          ___| |   \n");
					printf("         (___} |   \n"); 
					printf("         (___} |   \n");  
					printf("         (___} |   \n");   
					printf("          ``````   \n");
					printf("\n");
					printf(" 2.)      _____    \n");
					printf("         | ^ ^ |   \n");  
					printf("         |o   o|   \n");  
					printf("         | ooo |   \n");
					printf("          ``````   \n");
					printf("\n");
					printf(" 3.)     _______   \n");
					printf("        |  ___  |  \n");
					printf("        | |   | |  \n");
					printf("        | |  o| |  \n");
					printf("        | |   | |  \n");
					printf("        `````````  \n");
					printf("\n");
					printf(" 4.)     ______    \n");
					printf("        /     /    \n");
					printf("        \\     \\  \n");
					printf("        /     /    \n");
					printf("        \\     \\  \n");
					printf("         ``````    \n");
					printf("\n-------------------------------------------------\n");
					break;
				}
			}
		}
		
		int temp = 0; //<-- declaired here so that it resets to 0 every time so that the comparison between the user and the computers and numbers is easier 
		printf("\nScore: %d\n", score); 
		while(temp < turnWent){ //<-- makes sure the it asks for the users input as many times as the user completed a turn
			
			printf("\nWhat was the order that the shapes where presented?(enter a number 1 - 4): ");
			CharinInt = scanf("%d", &userIn);
			if(CharinInt != 1){ //<-- makes sure the user does not enter a char
				printf("\n-------------------------\n");
				printf("INVALID INPUT TRY AGAIN |");
				printf("\n-------------------------\n");
				scanf("%*[^\n]"); //<-- it makes shure that if the user enters a char then it will not get stuck in an infinate loop and ask the user for another input
			}
			else if(userIn > 4 || userIn <= 0){  //<-- makes sure the users input is between 1 and 4
				printf("\n-------------------------\n");
				printf("INVALID INPUT TRY AGAIN |");
				printf("\n-------------------------\n");
			}
			else{
				userHistory[temp] = userIn; //<-- saves the users inputs
				
				if(symbNum[temp] == userHistory[temp]){ //<-- compares the users inputs with the computers rand numbers if it does then it adds a score
					score ++;
				}
				
				temp ++;
			}	
		}
		
		turnComplete += 1;
		for(int i = 0; i<turnWent ;i++){
			if(symbNum[i] != userHistory[i]){ //<-- checks to see if all the users inputs mach with the computers rand generated numbers
				printf("\n-------------------------------------------------\n");
				printf("\nGAME OVER\n\nCorrect awnser is: ");
				for(int i = 0; i < turnWent; i++){
					printf("%d ", symbNum[i]); //<-- prints all the correct numbers 
				}
				printf("\n\nIt's cool you got to round %d tho and your score was %d\n", turnComplete, score);
				printf("\n-------------------------------------------------\n");
				
				gameOver = 1;
				break;
			}
		}
		turnWent += 1;
		
		if (turnWent == 99){
			printf("\n-------------------------------------------------\n");
			printf("\nCONGRADULATIONS YOU MADE IT TO THE MAX ROUNDS POSSIBLE\n");
			printf("\nYOUR MEMORY IS OUTSTANDING\n");
			ScoreBoardWrite(userName, score, gone);
			ScoreBoardPrint();
			printf("\n-------------------------------------------------\n");
			gameOver = 1;
		}
	}
	
	ScoreBoardWrite(userName, score, gone); //<-- writes the users name and score if they placed higher than someone else on the board 
	ScoreBoardPrint(); //<-- prints out the score board 
	printf("\n-------------------------------------------------\n");
	
	return 0;
}

void ScoreBoardPrint(){
	char items[82];
	int length;
	FILE *file = fopen("Score_board.txt", "r"); //<-- opens the file that was provided and reads it 
	
	if (file == NULL){ //<-- prints out an error message if the computer cannot find the file
		perror("Error opening file");
		exit(0); //<-- exits the program 
	}
	
	
	printf("\nHighscore Table:\n");
	printf("Rank  Name  Score   \n");
	
	int temp3 = 3;
	
	for(int i = 0; i < 9; i++){ //<-- prints out everything that is in the file (only for 9 lines though)
		fgets(items,82,file); //<-- gets everything until it hits a new line or a null character (i think \0)
		length = strlen(items);
		printf(" %d    ", i+1);
		for(int j = 0; j < length; j++){
			printf("%c",items[j]);
		}
	}
	
	fclose(file);
}

void ScoreBoardWrite(char* userName, int newScore, int *gone){
	int length;
	char items[8];
	struct userInfo info[18]; //<-- is 18 because when the name and score is being moved it needs to be able to have more structs so it does give an out of bounds error (i could have probable made it less than 18 but i just wanted to be sure)
	
	FILE* file = fopen("Score_board.txt", "r");
	
	if (file == NULL){
		perror("Error opening file");
		exit(0);
	}
	for(int i = 0; i < 9; i++){ //<-- had to do this because for some reason fscanf was scanning in a different language and random characters idk why 
		fgets(items,20,file);
		length = strlen(items);
		int e = 0;
		int f = 0;
		int num1;
		int num2;
		int num3;
		int hist = 0;
		for(int j = 0; j < length; j++){
			
			if(items[j] != ' '){ //<-- probably not necessary now that I think about it but was so that the computer would look at chars and ints without spaces  
				items[j]=toupper(items[j]); //<-- makes the scaned values uppercase (has no effect on numbers)
				int num = items[j]; //<-- scaned value is made into its ascii 
				if(num >= 65 && num <= 90){ //<-- checkes to see if it an uppercase letter using ascii 
					char letter = num;
					info[i].name[e] = letter; //<-- puts the char into the structs name variable char by char
					e ++;
				}
				else if(num >= 48 && num <= 57){ //<-- checks to see if its a number useing ascii and checks to see if the int is single didget double didget or tripple didget but it can only go up to tripple didget
					if (f == 0){
						num1 = num - 48;
						hist = num1;
					}
					else if (f == 1){
						num1 = num1 * 10;
						num2 = num - 48;
						hist = num1 + num2;
					}
					else if (f == 2){
						num1 = num1 * 10;
						num2 = num2 * 10;
						num3 = num - 48;
						hist = num1 + num2 + num3;
					}
					f++;
				}
			}
		}
		info[i].score = hist; //<-- appends the score to the score in the struct 
	}

	for (int i = 0; i < 9; i++){
		
		if ((info[i].score < newScore) && (gone == 0)){ //<-- ckecks to see if the users score is greater than any of the scores on the table 
			for(int j = 0; j < 9; j++){
				strcpy(info[i+j].tempName, info[i+j].name); //<-- it saves all the names and scores into a temporary variables 
				info[i+j].tempScore = info[i+j].score;
			}
			for(int j = 0; j < 9; j++){
				strcpy(info[i+1+j].name, info[i+j].tempName); //<-- it basically moves the names and scores that are below the score that use has down by one (only if the users score is higher than a different score that is on the leaderboard) 
				info[i+1+j].score = info[i+j].tempScore;
			}
			strcpy(info[i].name, userName); //<-- puts the users name and score on the leaderboard
			info[i].score = newScore;
			
			*gone ++; //<-- makes sure it only does this once (not entirely sure why I it a pointer)
		}	
	}
	
	
	
	
	fclose(file);
	file = fopen("Score_board.txt", "w"); //<-- clears the file then it prints to the file (if you want to append something to a file the use r+)
	for (int i = 0; i < 9; i++){  //<-- writes all the names and scores that are saved from struct 0-8 to the txt file
		for(int c = 0; c < 3; c++){
			fprintf(file,"%c", info[i].name[c]); //<-- had to print it character by character because if I didnt then it would print the letter d at the end of some of the names and a unicode at the last name for some reason
		}
		fprintf(file,"   %d\n",info[i].score);
	}
	fclose(file);
}