/********************************************************************************
* Author: Orion Crocker
* Filename: proof.c
* Date: 08/25/19
*
* Binary Clock
* Description: Binary Clock proof of concept, no where near finished product
*	code
********************************************************************************/

#include <stdio.h>
#include <time.h>
#include <unistd.h> // needed for sleep()

void bit(int number, int bits)
{
	for (unsigned i = 1 << (bits-1); i > 0; i=i/2)
		(number & i)? printf("1"): printf("0");
	printf("\n");
}

int main()
{
	time_t rawtime;
	struct tm *timeinfo;
	while(1)
	{
		// update time
		time(&rawtime);
		timeinfo = localtime(&rawtime);

		// output
		printf("%d:%d:%d\n",timeinfo->tm_hour,timeinfo->tm_min,timeinfo->tm_sec);
		printf("Hour:\t");
		bit(timeinfo->tm_hour,6);
		printf("Minute:\t");
		bit(timeinfo->tm_min,6);
		printf("Second:\t");
		bit(timeinfo->tm_sec,6);
		fflush(stdout);
		sleep(1);
	}
	return 0;
}
