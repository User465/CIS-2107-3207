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

int prompt();

int main(int argc, char* argv[])
{
  while(1) {
    int a = prompt();
    if(a == EXIT_SUCCESS)
    {
      return EXIT_SUCCESS;
    }
  } //end while
}

int prompt()
{
  char* argv[arrSize];
  char hostname[arrSize];
  char input[arrSize];
  char *token;
  const char t[] = {" -/\n"};
  gethostname(hostname, arrSize);
  printf("%s~>", hostname);

  if(fgets(input, sizeof(input), stdin) == NULL){
    puts(" ");
    return EXIT_SUCCESS;
  }
  token = strtok(input, t);

  int counter = 0;
  argv[counter] = token;
  counter++;

  if (strcmp(token, "exit") == 0 || feof(stdin)) {
    return EXIT_SUCCESS;
  }

  while (token != NULL) {
    token = strtok(NULL, t);
    argv[counter] = token;
    counter++;
  }

  // printf("you entered: ");
  // int i;
  // for (i = 0; i < counter - 1; ++i) {
  //     printf("%s ", argv[i]);

}//end while
