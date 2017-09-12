/*
Abstract of Approach

Techniques Used

Outline of Main Tasks of Each Process/Thread

Outline of Each Major Routine in Process/Thread

Input/Output of Each Function
**Include discussion of each possible function return value*/


#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/time.h>
#include <time.h>

int main(void)
{

  int i;
  char timerFile1[] = "times1.txt";

  for(i = 0; i < 10; i++)
  {
  	  pid_t pid;
  	  struct timeval start;
  	  gettimeofday(&start, NULL);

      FILE* times1;
      times1 = fopen(timerFile1, "w");
      fprintf(times1, "%ld\n", start.tv_usec);
      fclose(times1);

  	  pid = fork();

  	  if(pid < 0) //nope
      {
        	printf("\n The fork failed.\n");
      }

  		else if(pid == 0) //child Process
  		{
        char fileName[] = "file1.txt";

        printf("Start: %ld\n", start.tv_usec);
        int start = start.tv_usec;
        execlp("./application", "application", "fileName", "times1.txt", "1", start, NULL);
        printf("FAILED\n");
  		}

  		else //parent process stuff
  		{
        waitpid(pid, 0, WCONTINUED);
  		}

  }

}
