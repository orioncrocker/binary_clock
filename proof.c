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
#include <string.h>
#include <unistd.h> // needed for sleep()

#define BITS 6

char h[BITS+1] = {'\0'};
char m[BITS+1] = {'\0'};
char s[BITS+1] = {'\0'};

void num_to_bits(int number, char* arr)
{
  unsigned bit = 0;
  for (unsigned i = 1 << (BITS-1); i > 0; i=i/2) {
    arr[bit] = (number & i) ? '1' : '0';
    ++bit;
  }
}

void horizontal_output()
{
  printf("  ");
  for (unsigned i = 2; i < BITS; ++i)
    printf("%c", h[i]);
  printf(" H\n");
  printf("%s M\n", m);
  printf("%s S\n\n", s);
  fflush(stdout);
}

void vertical_output()
{
  for (int i = 0; i < BITS; i++) {
    printf("%c %c %c\n", h[i], m[i], s[i]);
  }
  printf("H M S\n\n");
  fflush(stdout);
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
    num_to_bits(timeinfo->tm_hour%12,h);
    num_to_bits(timeinfo->tm_min,m);
    num_to_bits(timeinfo->tm_sec,s);

    printf("%d:%d:%d\n",timeinfo->tm_hour%12,timeinfo->tm_min,timeinfo->tm_sec);
    horizontal_output();
    //vertical_output();
    sleep(1);
  }
  return 0;
}
