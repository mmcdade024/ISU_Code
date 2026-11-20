
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <ncurses/ncurses.h>
#define MAXWORDS 100
#define WORDLEN 11
#define DEBUG 0   // set to 0 to disable debug output

// reads words from the file
// into wl and trims the whitespace off of the end of each word
// DO NOT MODIFY THIS Prototype
int readWords(char* wl[MAXWORDS], char* filename); 

//modifies s to trim white space off the right side
// DO NOT MODIFY THIS Prototype
void trimws(char* s) ;

int main(int argc, char* argv[]) {
	char* wordlist[MAXWORDS];
	int wordCount;
	int i;
	wordCount = readWords(wordlist, argv[1]);
	char character[2] = "<-";

	if (DEBUG) {
		printf("Read %d words from %s \n",wordCount, argv[1]);
		
		printf("Now printing the words:\n");
		
		for(i=0;i<wordCount;i++){
			printf(":%s" , wordlist[i]);
		}
		
	}

// most of your code for part 2 goes here. Don't forget to include the ncurses library 
	initscr();
	int ROW =0;
	int COL =0;
	i=0;
	for(ROW=0; ROW<16; ROW++){
		for(COL=0; COL<75; COL+=15){
			if(i<76){
				mvprintw(ROW,COL,"%15s", wordlist[i]);
				i++;
			}
		}
	}
	refresh();
	
	int time = 0;
	int t1 = 0;
	int tri, cir, bx, sq;
	int l_hori, l_vert, r_hori, r_vert;
	int last_length[20];
	int history_index=0;
	int wrd_index=0;
	ROW=0;
	COL=15;
	mvprintw(ROW,COL,"<-");
	//track cursor when printing sentence: (this is same as saying which column to start printing)
	int cursor = 0;
	
	
	while(1){
		refresh();
		
		scanf("%d,",&time);
		scanf("%d, %d, %d, %d,",&tri, &cir, &bx, &sq);
		scanf("%d, %d, %d, %d", &l_hori, &l_vert, &r_hori, &r_vert);
		
		if((time-t1)>120){ //allow user to control after 120ms has passed.
			if(l_vert < -100){//left stick up:
				mvprintw(ROW,COL,"  ");
				ROW--;
				mvprintw(ROW,COL,"<-");
			}
			if(l_vert > 100){ //left stick down
				mvprintw(ROW,COL,"  ");
				ROW++;
				mvprintw(ROW,COL,"<-");	
			}
			if(l_hori<-100){ //left stick left

				mvprintw(ROW,COL,"  ");
				COL= COL-15;
				mvprintw(ROW,COL,"<-");				
			}
			if(l_hori>100){ //left stick right
				mvprintw(ROW,COL,"  ");
				COL= COL+15;
				mvprintw(ROW,COL,"<-");
			}

			//add without space:
			if(sq){
				
				wrd_index = (ROW*5) + (COL/15)-1;
				//print word at this cursor location
				mvprintw(20,cursor,"%s",wordlist[wrd_index]);
				//move the cursor to the end of the word.
				cursor = cursor + strlen(wordlist[wrd_index]);
				//keep history of the length of last word/action
				last_length[history_index] = strlen(wordlist[wrd_index]);
				history_index++; //increment the index for the next item to be stored
				refresh();
			}
			
			if(tri){
				
				//get index of the word pointed by the "<-"
				wrd_index = (ROW*5) + (COL/15)-1;
				//print word at this cursor location
				mvprintw(20,cursor,"%s ",wordlist[wrd_index]);
				//move the cursor to the end of the word.
				cursor = cursor + strlen(wordlist[wrd_index]) + 1;
				//keep history of the length of last word/action
				last_length[history_index] = strlen(wordlist[wrd_index]) + 1;
				history_index++; //increment the index for the next item to be stored
				refresh();
			}
			
			
			//undo last item
			if(bx){
				//get the index of the last added item
				history_index = history_index - 1;
				//check length of last item, replace with spaces
				int l_len = last_length[history_index];
				//now we move the cursor back column by column, replacing with spaces
				for(int k=0; k<l_len; k++){
					cursor--;
					mvprintw(20, cursor, " ");
				}
				refresh();
			}
			
			t1 = time;
		}
		
		//wait till all buttons reset:
		/*while(tri!=0 || cir!=0 || bx!=0 || sq!=0){
			scanf("%d,",&time);
			scanf("%d, %d, %d, %d,",&tri, &cir, &bx, &sq);
			scanf("%d, %d, %d, %d", &l_hori, &l_vert, &r_hori, &r_vert);
			t1 = time;
		}*/
		
	}

    return 0;
	}

int readWords(char* wl[MAXWORDS], char* filename){
	
	FILE* file = fopen(filename, "r");
	char currentword[WORDLEN];
	int count=0;
	
	//dynamically allocate array of pointers:
	
	//while not reached end of file
	while(feof(file)!=1){
		
		//fgets(currentword, WORDLEN, file);
		fscanf(file, "%s", currentword);
		
		//allocate memory in wl[]. malloc size of word + 1 (plus 1 for '\0' character) 
		//sizeof(char) is 1 byte, so its redundant here
		wl[count] = malloc(strlen(currentword)+1);
		
		//now place the word into that newly allocated space inside w1:
		strcpy(wl[count],currentword);
		
		//increase the counter/iterator
		count++;
		
	}
	fclose(file);
	return count;
	
}

void trimws(char* s){
	
	//start checking for spaces from end of sentence, or say goodbye to your sentence.
	for(int i=strlen(s)-1;i>=0;i--){
		
		if(isspace(s[i])){
			s[i]='\0';
		}
		else{
			break;
		}
		
	}
}