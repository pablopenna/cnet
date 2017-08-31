#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

char * getMyDate()
{
		time_t t = time(NULL);
		struct tm time = *localtime(&t);

		//printf("%d-%d-%d %d:%d:%d\n", time.tm_year + 1900, time.tm_mon + 1, time.tm_mday, time.tm_hour, time.tm_min, time.tm_sec);
		//printf("daylight saving time activated?: %d\n", time.tm_isdst);
		
		char * placeholder = (char*) malloc(64*sizeof(char));
		sprintf(placeholder,"%d-%d-%d %d:%d:%d\n", time.tm_year + 1900, time.tm_mon + 1, time.tm_mday, time.tm_hour, time.tm_min, time.tm_sec);

		return placeholder;
}

/*
int main()
{
	printf("%s",getMyDate());
	return 0;
}
*/
