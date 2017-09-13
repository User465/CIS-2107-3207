/*
Abstract of Approach
This program basically forks a process into a parent and child process, but only
one of those processes is executing the application.c program (that is, the child
process is the one executing application). The start time is taken before the fork,
and the end time is taken within application (as explained in the application program).
With this, the start time is written to the file and the file is passed as an argument.
As for the processes, the parent process merely has a waitpid statement so it can
allow for the child process to completely finish before moving on to the parent
process (which would signal the end of the program). Within the child process,
start time is printed to the screen and application is run with 4 arguments which
will be explained below. After everything is finished, the times file is read
so the start and end times can be substracted so the differences can be printed
to the screen.

Techniques Used
I used the main function to complete everything I needed, only because I felt
getting functions involved was going to be a lot to keep track of. I organized
my code in a way so it's easy to navigate the processes (especially since there's
only one).

Outline of Main Tasks of Each Process/Thread
The main task of the parent process is to wait for the child process. The main
task of the child function is to execute the application program so the process
is replaced with the program. It calls the exec function with the application name
as well as "fileName", "times1.txt", "1". These correspond to the file name with
the record, the text file that contains the start and end times, and the ID number
to keep track of which timer program I am running (in this case, timer1 will just
have an ID of one and the file name is called times1.txt with a lowercase t).

Outline of Each Major Routine in Process/Thread
The major routine in the child process is as follows: file name is created, start
time is printed to the screen to make sure it was successful, and execlp is called
with the arguments detailed in the previous paragraph.

Input/Output of Each Function
**Include discussion of each possible function return value
As for this program, I decided not to have any helper functions because I did
not feel I could keep track of everything a function would require (such as
arguments and return values). Instead I put all the action into the main function,
so main function handles the processes and forks and file creations and writing
the time difference to the appropriate file.*/



#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/time.h>
#include <time.h>

#define ITERATIONS 50 //definitive number of iterations for the histogram.

int main(void)
{

  int i;
  char timerFile1[] = "times1.txt"; //file name specific to this program. DIFFERENT FROM TIMER2 FILE NAMES

  /*The following statements reset any trailing data within the file from
    previous use so the data written isn't ruined.*/
  FILE* fpp;
  fpp = fopen(timerFile1, "w");
  fclose(fpp);

  for(i = 0; i < ITERATIONS; i++) //iterating the entire forking process.
  {
      /*These statements catalogue the start time before the fork
        The time is also written to the specified file.*/
  	  pid_t pid;
  	  struct timeval start;
  	  gettimeofday(&start, NULL);
      FILE* times1;
      times1 = fopen(timerFile1, "a");
      fprintf(times1, "%ld\n", start.tv_usec);
      fclose(times1);

  	  pid = fork(); //forked process

  	  if(pid < 0) //unsuccessful fork
      {
        	printf("\n The fork failed.\n");
      }

  		else if(pid == 0) //child Process
  		{
        char fileName[] = "file1.txt"; //specific file name for the application to handle

        printf("Start: %ld\n", start.tv_usec); //prints the start time to the screen
        execlp("./application", "application", "fileName", "times1.txt", "1", NULL); //executes application with the following arguments
        printf("FAILED\n");
  		}

  		else //parent process
  		{
        waitpid(pid, 0, WCONTINUED); //lets child process completely finished before moving onto this process.
  		}

  }

  int differenceArray[4][ITERATIONS]; //holds all the start and end times

  int j;
  FILE* fp;
  fp = fopen("times1.txt", "r+"); //opens the times file so the differences can be made

  for(j = 0; j <= ITERATIONS; j++) //iterates through all the pairs to store them into the array
  {
    fscanf(fp, "%d", &differenceArray[0][j]); //saves the start
    fscanf(fp, "%d", &differenceArray[1][j]); //saves the end
  }
  fclose(fp);

  int m;
  printf("%s", "Differences for Timer\n"); //printed to the screen
  for(m = 0; m < ITERATIONS; m++)
  {
  printf("%d\n", differenceArray[1][m] - differenceArray[0][m]); //prints the difference to the screen with end - start. 
  }

}
