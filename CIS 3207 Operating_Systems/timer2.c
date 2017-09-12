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

int main(void){

  int i;

  for(i = 0; i < 10; i++)
  {

    pid_t pid;
    struct timeval start_time_1;
    gettimeofday(&start_time_1, NULL);
    printf("Start Time 1: %ld\n", start_time_1.tv_usec);

    pid = fork();

    if(pid < 0) //not successful fork
    {
        printf("\n The fork failed.\n");
    }

    else if(pid == 0) //child Process 1
      {
        char childProcess1[] = "Times1.txt";
        FILE* childProcessTimes1;
        childProcessTimes1 = fopen(childProcess1, "a");
        fprintf(childProcessTimes1, "%ld\n", start_time_1.tv_usec);
        fclose(childProcessTimes1);

        printf("%s\n", " ");

        char fileName[] = "File1.txt";
        int start = (int) start_time_1.tv_usec;

        execlp("./application", "application", "fileName", "Times1.txt", "1", start, NULL);
        printf("FAILED\n");
      }


    else //parent process stuff
      {
        struct timeval start_time_2;
        gettimeofday(&start_time_2, NULL);
        printf("Start Time 2: %ld\n", start_time_2.tv_usec);

        pid_t pid2;
        pid2 = fork();

        if(pid2 < 0) //unsuccessful fork
        {
          printf("\n The fork failed.\n");
        }

        else if(pid2 == 0) //child Process 2
          {
          char childProcess2[] = "Times2.txt";
          FILE* childProcessTimes2;
          childProcessTimes2 = fopen(childProcess2, "a");
          fprintf(childProcessTimes2, "%ld\n", start_time_2.tv_usec);
          fclose(childProcessTimes2);

          printf("%s\n", " ");

          char fileName[] = "File2.txt";
          int start = (int) start_time_2.tv_usec;

          execlp("./application", "application", "fileName", "Times2.txt", "2", start, NULL);
          printf("FAILED\n");
          }

        else //parent process
        {
          waitpid(pid, 0, WCONTINUED);
          waitpid(pid2, 0, WCONTINUED);
          printf("%s", "This is the Parent Process.\n");
        }

      }
  }
  }
