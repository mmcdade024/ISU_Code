#include <stdio.h>
int main(void) {
	double TestScore1 ;
	double TestScore2 ;
	double TestScore3 ;
	double TestScore4 ;
	double TestScore5 ;
	double classAvg ;
	
	printf("Enter test score 1: \n");
	scanf("%lf", &TestScore1);
	printf("Enter test score 2: \n");
	scanf("%lf", &TestScore2);
	printf("Enter test score 3: \n");
	scanf("%lf", &TestScore3);
	printf("Enter test score 4: \n");
	scanf("%lf", &TestScore4);
	printf("Enter test score 5: \n");
	scanf("%lf", &TestScore5);
	
	classAvg = (double)(TestScore1 + TestScore2 + TestScore3 + TestScore4 + TestScore5)/5;
	
	printf("Test Score 1: %lf\n", TestScore1);
	printf("Test Score 2: %lf\n", TestScore2);
	printf("Test Score 3: %lf\n", TestScore3);
	printf("Test Score 4: %lf\n", TestScore4);
	printf("Test Score 5: %lf\n", TestScore5);
	printf("average: %lf", classAvg);
	
	return 0;
}