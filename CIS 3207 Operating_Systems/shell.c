/*
Abstract of Approach

Techniques Used

Outline of Main Tasks of Each Process/Thread

Outline of Each Major Routine in Process/Thread

Input/Output of Each Function
**Include discussion of each possible function return value

*/
#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <sys/time.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <dirent.h>
#include "builtin.h"

#define arrSize 1024

char* readALine();
void printHostName();
int evaluate(char *line);
void parseLine(char *line, char* argv[arrSize]);
void forkProgram(char* argv[arrSize]);
int runBuiltIn(char* argv[arrSize]);

int main(int argc, char* argv[])
{
  char *lineInput;
  system("clear");
  while(1) {
    printHostName();
    lineInput = readALine();
    if(strcmp(lineInput, "quit\n") == 0 || feof(stdin))
    {
      return EXIT_SUCCESS;
    }
    int a = evaluate(lineInput);
  } //end while
}

char *readALine()
{
  char input[arrSize];
  char *lineRead;

  fgets(input, sizeof(input), stdin);
  lineRead = input;
  return lineRead;
}

void printHostName()
{
  char hostname[arrSize];
  char cwd[1024];
  if (getcwd(cwd, sizeof(cwd)) != NULL)
  {
    gethostname(hostname, arrSize);
    printf("%s%s~>", hostname, cwd);
  }
   else
       perror("getcwd() error");
}

int evaluate(char *line)
{
  char* argv[arrSize]; //array of pointers to chars
  parseLine(line, argv);
  int a = isBuiltIn(argv);

  if(argv[0] == NULL)
  {
    return 0; //ignore empty lines
  }

  if (a == 1)
  {
    runBuiltIn(argv);
  }
  else if (a == 0)
  {
    printf("%s", "hey");
    forkProgram(argv);
    return 1;
  }
  return 1;
}

void parseLine(char *line, char* argv[arrSize])
{
  char* token;
  const char t[] = {" -/\n"};
  int counter = 0;

  token = strtok(line, t);
  argv[counter] = token;
  counter++;

  while (token != NULL)
  {
    token = strtok(NULL, t);
    argv[counter] = token;
    counter++;
  }
}

int runBuiltIn(char* argv[arrSize])
{
  int i;
  for(i = 0; i < sizeof(builtin_command) / sizeof(builtin_command[0]); i++)
  {
    if(strcmp(argv[0], builtin_command[i]) == 0)
    {
      return (*builtin_function[i])(argv);
    }
  }
  return 0;
}

void forkProgram(char* argv[arrSize])
{
  pid_t pid;
  pid = fork();

  if(pid < 0)
  {
    printf("%s", "Error, can't fork");
  }

  else if(pid == 0) //child process
  {
    if(execvp(argv[0], argv) < 0)
    {
      printf("%s: Command not found", argv[0]);
      exit(0);
    }
  }

  else //parent process
  {
    int status;
    if(waitpid(pid, &status, 0) < 0)
    {
      printf("%s", "ERROR");
    }
  }


}
