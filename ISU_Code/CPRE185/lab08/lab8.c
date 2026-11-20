// WII-MAZE Skeleton code written by Jason Erbskorn 2007
// Edited for ncurses 2008 Tom Daniels
// Updated for Esplora 2013 TeamRursch185
// Updated for DualShock 4 2016 Rursch


// Headers
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <ncurses/ncurses.h>
#include <unistd.h>
#include <time.h>
#include <ctype.h>
#include <sys/time.h>

// Mathematical constants
#define PI 3.14159

// Screen geometry
// Use ROWS and COLS for the screen height and width (set by system)
// MAXIMUMS
#define NUMCOLS 100
#define NUMROWS 72

// Character definitions taken from the ASCII table
#define AVATAR 'A'
#define WALL '*'
#define EMPTY_SPACE ' '

// 2D character array which the maze is mapped into

char MAZE[NUMROWS][NUMCOLS];

// POST: Generates a random maze structure into MAZE[][]
//You will want to use the rand() function and maybe use the output %100.  
//You will have to use the argument to the command line to determine how 
//difficult the maze is (how many maze characters are on the screen).
void generate_maze(int difficulty);

// PRE: MAZE[][] has been initialized by generate_maze()
// POST: Draws the maze to the screen 
void draw_maze(void);

// PRE: 0 < x < COLS, 0 < y < ROWS, 0 < use < 255
// POST: Draws character use to the screen and position x,y
void draw_character(int x, int y, char use);

// PRE: -1.0 < x_mag < 1.0
// POST: Returns tilt magnitude scaled to -1.0 -> 1.0
// You may want to reuse the roll function written in previous labs.  
float calc_roll(float x_mag);

// Main - Run with './ds4rd.exe -t -g -b' piped into STDIN
int main(int argc, char* argv[])
{
	int lastT = 0;
	int t;
	int b1;
	int b2;
	int b3;
	int b4;
	int i = 36;
	int j = 0;
	int z;
	int x;
	
	if (argc <2) { printf("You forgot the difficulty\n"); return 1;}
	int difficulty = atoi(argv[1]); // get difficulty from first command line arg
	// setup screen    
	initscr();
	refresh();

	// Generate and draw the maze, with initial avatar
	generate_maze(difficulty);
	draw_maze();
	// Read gyroscope data to get ready for using moving averages.    

	// Event loop
	do
	{
	
		scanf("%d, %d, %d, %d, %d", &t, &b1, &b2, &b3, &b4); // b1= triange, b2 = O, b3 = X, b4 = square
		draw_character(i,j,AVATAR);
		
		if ((t -  lastT) >= 250){
			
			if ((b2 == 1) && (MAZE[i+1][j] != WALL)){
				draw_character(i,j,EMPTY_SPACE);
				draw_character(++i,j,AVATAR);
			}
			else if ((b4 == 1) && (MAZE[i-1][j] != WALL)){
				draw_character(i,j,EMPTY_SPACE);
				draw_character(--i,j,AVATAR);
			}
			else if (MAZE[i][j+1] != WALL){
				draw_character(i,j,EMPTY_SPACE);
				draw_character(i,++j,AVATAR);
			}
			if ((i == 72) || (i == 0)){
				endwin();
				printf("\nDont go out of bounds!\n");
				break;
			}
			if (j == 100){
				endwin();
				printf("YOU WIN!\n");
				break;
			}
			if (b1 == 1){
				endwin();
				printf("\n\n\nYou held the triangle button down there for the game has ended\n");
				printf("\nYou Lost!\n");
				fflush(stdout);
				break;
			}
			if (MAZE[i][j+1] == WALL && MAZE[i-1][j] == WALL && MAZE[i+1][j] == WALL){
				endwin();
				printf("You ran into a small bucket ! :(\n");
				break;
			}
			if (MAZE[i][j+1] == WALL){
				z = i;
				x = i;
				
				while(MAZE[++z][j] == EMPTY_SPACE){}
				while(MAZE[--x][j] == EMPTY_SPACE){}

				while((x < i) && (MAZE[++x][j+1] == WALL)){}
				while((z > i) && (MAZE[--z][j+1] == WALL)){}
				
				if ((x == i) && (z == i)){
					endwin();
					printf("You ran into a large bucket ! :(\n");
					break;	
				}
			}
			
			lastT = t;
			fflush(stdout);
		}
		// Read data, update average
		
		// Is it time to move?  if so, then move avatar

	} while(1); // Change this to end game at right time 
	
	endwin();
		
	// Print the win message
	
	return 0;
}


// PRE: 0 < x < COLS, 0 < y < ROWS, 0 < use < 255
// POST: Draws character use to the screen and position x,y
//THIS CODE FUNCTIONS FOR PLACING THE AVATAR AS PROVIDED.
//
//    >>>>DO NOT CHANGE THIS FUNCTION.<<<<
void draw_character(int x, int y, char use)
{
	mvaddch(y,x,use);    
	refresh();
}

void generate_maze(int difficulty){
	srand(time(NULL));
	
	for (int i = 0; i < NUMROWS; i++){
		for (int j = 0; j < NUMCOLS; j++){
			if (rand()%100 < difficulty){ 
				MAZE[i][j] = WALL;
			}
			else 
				MAZE[i][j] = EMPTY_SPACE;
		}
	}
}

void draw_maze(void){
	for (int i = 0; i < NUMROWS; i++){
		for (int j = 0; j < NUMCOLS; j++){
			draw_character(i , j, MAZE[i][j]);
		}
	}	
}