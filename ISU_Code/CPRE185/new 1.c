#include <stdio.h>

int main(){
	char list[] = {"like", "dog", "cat"};
	
	printf("%c", toupper(list[1][0]));
	
	return 0;
}