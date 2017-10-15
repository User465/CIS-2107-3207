/*
Abstract of Approach
Timer2 is similar to Timer1, with the exception that there are two forks in This
program. The start_time_1 is taken before the first fork is launched, then a
child process 1 is created (as well as the original parent process). Within
child process 1, application is executed and the end time is recorded. As with
the parent process, start_time_2 is created before the 2nd fork. In child process
2 (which was created from the fork), application is also executed with the end
time recorded from within. The parent process waits for this process to finish
before calculating the differences between each respective start and end time.

Techniques Used
I also decided to keep this all in the main function. I loop the entire program
50 times so the start and end times are printed to the screen as well as the
differences so I can easily put them in an excel sheet to make the histogram.
I organized my code so it should be easy to navigate despite the large block
of code in the main function.

Outline of Main Tasks of Each Process/Thread
Child Process 1: this process catalogues the start time of its process before
executing the application. The arguments that are passed are the file name for
the record, the times file, and the id number to specify the file application is
writing to. In Child Process 2, Application is also executed as well as the times
being logged into the respective files. Parent process only waits until the Child
processes are done before writing the differences.

Outline of Each Major Routine in Process/Thread
Child Process 1: Open times1 text file, log in start time, then execute application
Child Process 2: Open times2 text file, log in start time, then execute application

Input/Output of Each Function
**Include discussion of each possible function return value
  There is only the main function which runs each process. It takes no Input
  and just has each of the files created. */



#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/time.h>
#include <time.h>

#define ITERATIONS 1000 //the loop

int main(void){

  int i;

  /*The following clears all data from the files so it's
    nice and new and fresh every time the program runs. */
  FILE* fpp;
  fpp = fopen("Times1.txt", "w");
  fclose(fpp);
  fpp = fopen("Times2.txt", "w");
  fclose(fpp);

  for(i = 0; i < ITERATIONS; i++) //loops entire program
  {

    /*Takes log of the start time before the fork and
    prints it to the screen.*/
    pid_t pid;
    struct timeval start_time_1;
    gettimeofday(&start_time_1, NULL);

    pid = fork();

    if(pid < 0) //not successful fork
    {
        printf("\n The fork failed.\n");
    }

    else if(pid == 0) //child Process 1
      {
        execlp("./application", "application", "File1.txt", "Times1.txt", "1", NULL); //executes the application
        printf("FAILED\n");
      }


    else //parent process stuff
      {
        struct timeval start_time_2;
        gettimeofday(&start_time_2, NULL); //gets time 2

        pid_t pid2;
        pid2 = fork(); //second fork

        if(pid2 < 0) //unsuccessful fork
        {
          printf("\n The fork failed.\n");
        }

        else if(pid2 == 0) //child Process 2
          {
          execlp("./application", "application", "File2.txt", "Times2.txt", "2", NULL);
          printf("FAILED\n");
          }

        else //parent process
        {
          /*Opens the first timer and writes the time to the file */
          char childProcess1[] = "Times1.txt";
          FILE* childProcessTimes1;
          childProcessTimes1 = fopen(childProcess1, "a");
          fprintf(childProcessTimes1, "%ld\n", start_time_1.tv_usec);
          fclose(childProcessTimes1);

          /*Opens the second timer and writes the time to the file */
          char childProcess2[] = "Times2.txt";
          FILE* childProcessTimes2;
          childProcessTimes2 = fopen(childProcess2, "a");
          fprintf(childProcessTimes2, "%ld\n", start_time_2.tv_usec);
          fclose(childProcessTimes2);

          // waits for the other processes to finish before running.
          waitpid(pid, 0, WCONTINUED);
          waitpid(pid2, 0, WCONTINUED);
        }
      }
  }

  int differenceArray[4][ITERATIONS]; //keeps track of the start and end time differences.

  int j;
  FILE* fp;
  fp = fopen("Times1.txt", "r+"); //this and the following for loop concerns the first timer process stuff

  for(j = 0; j <= ITERATIONS; j++)
  {
    fscanf(fp, "%d", &differenceArray[0][j]);
    fscanf(fp, "%d", &differenceArray[1][j]);
  }
  fclose(fp);

  int k;
  FILE* fp2;
  fp2 = fopen("Times2.txt", "r+"); //this handles the second process start and end time

  for(k = 0; k <= ITERATIONS; k++)
  {
    fscanf(fp2, "%d", &differenceArray[2][k]);
    fscanf(fp2, "%d", &differenceArray[3][k]);
  }
  fclose(fp2);

  int n;

  FILE* diff;
  diff = fopen("Differences2.txt", "w");
  fclose(diff);
  diff = fopen("Differences2.txt", "a+");

  //printf("%s", "Differences for Timer 2\n"); //prints the 2nd child process differences to the screen.
  for(n = 0; n < ITERATIONS; n++)
  {
  //printf("%d\n", abs(differenceArray[3][n] - differenceArray[2][n]));
  fprintf(diff, "%d\n", abs(differenceArray[3][n] - differenceArray[2][n]));
  }
  fclose(diff);
  }
