#include <stdio.h>
#include <ctype.h>
#include <string.h>

int main(){
	char list[40] = {"dog"}, {"cat"};
	
	printf("%c", toupper(list[1][0]));
	
	return 0;
}