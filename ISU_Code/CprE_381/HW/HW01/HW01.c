#include <stdio.h>

int main() {
	
    int vals[] = {25, 1, 4, 10, 381, 42, 100, 60, 0, 12, 25};
    int input1, input2, output;

    printf("Please enter a number:\n");
    scanf("%d", &input1);

    printf("Please enter a number:\n");
    scanf("%d", &input2);

    int shifted_input1 = input1 << 5;

    int shifted_input2 = input2 << 2;

    int array_value = vals[shifted_input2 / 4];

    output = shifted_input1 + array_value;

    printf("%d\n", output);

    return 0;