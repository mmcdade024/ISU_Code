/* strrchr example */
#include <stdio.h>
#include <string.h>

char *str_chr(char *str, int character);

int main ()
{
  char str[] = "This is a sample string";
  char * pch;
  pch=str_chr(str,'s');
  printf ("Last occurence of 's' found at %d \n",pch-str+1);
  return 0;
}

char *str_chr(char *str, int character) {
    while (*str != '\0') {
        if (*str == character) {
            return str;
        }
        str++;
    }
    return NULL;
}