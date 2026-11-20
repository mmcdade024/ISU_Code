/*---------------------------------------------------------------------------
-		                SE 185: Lab 09 - The DS4Talker                      -
-	Name: Mikhail McDade                                                    -
- 	Section: 1                                                              -
-	NetID: mamcdade                                                         -
-	Date: 4/11/2023                                                         -
----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------
-								Includes									 -
-----------------------------------------------------------------------------*/
#include <ctype.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ncurses/ncurses.h>

/*----------------------------------------------------------------------------
-								Defines	    								 -
-----------------------------------------------------------------------------*/
#define MAX_WORD_LENGTH 12 /* The maximum length a word can be. */
#define MAX_WORDS 75 /* The maximum words that can be in the DS4Talker. */

/*----------------------------------------------------------------------------
-	                            Prototypes                                   -
-----------------------------------------------------------------------------*/
void trim_whitespace(char *string);

int read_words(char *word_list[MAX_WORDS], char *file_name);

/*----------------------------------------------------------------------------
-	                                Notes                                    -
-----------------------------------------------------------------------------*/
// Compile with gcc lab09.c -o lab09 -lncurses
// Run with ./ds4rd.exe -d 054c:05c4 -D DS4_BT -t -b -bt -bd | ./lab09 word_list.txt
// NO GLOBAL VARIABLES ARE ALLOWED!

/*----------------------------------------------------------------------------
-								Implementation                               -
-----------------------------------------------------------------------------*/
int main(int argc, char *argv[])
{
    char *word_list[MAX_WORDS];
    int word_count = read_words(word_list, "word_list.txt");
	int index = 0; 
	int pr_index = 0;
	int i = 0;
	int cursor = 0;
	int pr_item[40];
	
    initscr();
	
	int time;
	int final_time = 0;
	
	int X;
	int O;
	int triangle;
	int square;
	
	int R3;
	int L3;
	int options;
	int share;
	int R2;
	int L2;
	int R1;
	int L1;
	
	int up;
	int down;
	int left;
	int right;
	
	int row = 0; 
	int col = 0;
	
	row = 0; 
	col = 15;
	int z = 0
	mvprintw(row, col,"<-");
	refresh();
	
	for(row=0; row<16 ; row++){
		for(col=0; col<75; col+=15){
			if(z < 76){
				mvprintw(row, col, "%15s", word_list[i]);
				i++;
			}
		}
		refresh();
	}
	
	while(1){
		
		scanf("%d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d",
		&time, &triangle, &O, &X, &square, &R3, &L3, &options, &share, &R2, &L2, &R1, &L1, &up, &left, &down, &right);
		
		if (time - final_time > 150){
			
			if (up == 1 && row > 0){
				mvprintw(row, col, "  ");
				row--;
				mvprintw(row, col, "<-");
			}
			if (down == 1 && row < 15){
				mvprintw(row, col, "  ");
				row++;
				mvprintw(row, col, "<-");
			}
			if(right == 1 && col < 75){
				mvprintw(row, col, "  ");
				col+=15;
				mvprintw(row, col, "<-");
			}
			if(left == 1 && col > 0){
				mvprintw(row, col, "  ");
				col-=15;
				mvprintw(row, col, "<-");
			}
			
			refresh();
			
			if(triangle == 1){
				index = (row*5) + (col/15)-1;
				mvprintw(20,cursor,"%s ",word_list[index]);
				cursor = cursor + strlen(word_list[index]) +1;
				pr_item[pr_index] = strlen(word_list[pr_index]) +1;
				pr_index++;
				refresh();
			}
			
			if(X == 1){
				index -= 1;
				int length = pr_item[index];
				for(int i = 0; i < length; i++){
					cursor --;
					mvprintw(20, cursor, " ");
				}
				refresh();
			}
			if(square == 1){
				index = (row*5) + (col/15)-1;
				mvprintw(20,cursor,"%s",word_list[index]);
				cursor = cursor + strlen(word_list[index]);
				pr_item[pr_index] = strlen(word_list[pr_index]);
				pr_index ++;
				refresh();
			}
			if(O == 1){
				index = (row*5) + (col/15)-1;
				word_list[index][0] = toupper(word_list[index][0]);
				mvprintw(20,cursor,"%s ",word_list[index]);
				cursor = cursor + strlen(word_list[index]) +1;
				pr_item[pr_index] = strlen(word_list[pr_index]) +1;
				pr_index++;
				refresh();
			}
			if((options == 1) || (share == 1)){
				break;
			}
			final_time = time;
		}
	}
	
	endwin();
	
    return 0;
}

/**
 * Modifies the given string to trim white space off at the end.
 * DO NOT MODIFY THIS FUNCTION.
 *
 * @param string - The string to ensure has no whitespace.
 */
 
void trim_whitespace(char *string)
{
    int length = strlen(string);
    if (length == 0) return;

    int i = length - 1;
    while (isspace(string[i]) && i >= 0)
    {
        string[i] = '\0';
        i--;
    }
}

/**
 * Reads words from the given file into word_list.
 * DO NOT MODIFY THIS FUNCTION.
 *
 * @param word_list - The list of words where the words taken from the given
 *                    file will be placed.
 * @param file_name - The name of the file to read from.
 * @return - The number of words read from the file.
 */
int read_words(char *word_list[MAX_WORDS], char *file_name)
{
    int number_of_words_read = 0;
    char line[MAX_WORD_LENGTH];
    char *pointer;
    FILE *file = fopen(file_name, "r");

    while (!feof(file))
    {
        pointer = fgets(line, MAX_WORD_LENGTH, file);
        if (pointer != NULL)
        {
            trim_whitespace(line);
            word_list[number_of_words_read] = (char *) malloc(strlen(line) + 1);
            strcpy(word_list[number_of_words_read], line);
            number_of_words_read++;
        }
    }

    fclose(file);
    return number_of_words_read;
}
