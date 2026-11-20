int  main(){
	int milliSec;
	int sec;
	int min;
	int hour;
	
	printf("Enter time in milliseconds: ");
	scanf("%d", &milliSec);
	
	milliSec = milliSec % 1000;
	sec = milliSec / 1000;
	min = sec / 60;
	sec = sec - (min * 60);
	hour = min / 60;
	min = min - (hour * 60);
	
	printf("millisec = %d\n", milliSec);
	printf("sec = %d\n", sec);
	printf("min = %d\n", min);
	printf("hour = %d\n", hour);
	
	return 0;
}
	