/*
Abstract of Approach

Techniques Used

Outline of Main Tasks of Each Process/Thread

Outline of Each Major Routine in Process/Thread

Input/Output of Each Function
**Include discussion of each possible function return value

*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <sys/time.h>
#include <unistd.h>
#include <string.h>

#define arrSize 1024

char* readALine();
void printHostName();
void evaluate(char *line);
void parseLine(char *line, char* argv[arrSize]);

int main(int argc, char* argv[])
{
  char *lineInput;
  while(1) {
    printHostName();
    lineInput = readALine();
    if(strcmp(lineInput, "exit\n") == 0 || feof(stdin))
    {
      puts(" ");
      return EXIT_SUCCESS;
    }
    evaluate(lineInput);
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

void evaluate(char *line)
{
  char* argv[arrSize]; //array of pointers to chars

  parseLine(line, argv);

  //rest of code!

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
