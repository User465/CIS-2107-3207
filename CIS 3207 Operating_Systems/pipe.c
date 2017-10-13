
#include "pipe.h"


int main(int argc, char *argv[])
{
  return 1;
}


int runBuiltIn(char* argv[])
{
    int i;
    for(i = 0; i < commandSize; i++)
    {
        if(strcmp(argv[0], builtin_command[i]) == 0)
        {
            return (*builtin_function[i])(argv);
        }
    }
    return 0;
}



void pipeEvaluate(char* leftSide[arrSize], char* rightSide[arrSize])
{
  int fd[2];
  pid_t pid;
  pipe(fd);
  pid = fork();

  if(pid < 0)
  {
    perror("Error forking");
    exit(1);
  }

  if(pid == 0) //child process
  {
    dup2(fd[1], 1);

    if(isBuiltIn(leftSide) == 1)
    {
      runBuiltIn(leftSide);
    }

    else
    {
      if(execvp(leftSide[0], leftSide) < 0)
      {
        printf("%s", "Error exec");
      }
    }
  }

  else //parent process
  {
    dup2(fd[0], 0);

    if(isBuiltIn(rightSide) == 1)
    {
      runBuiltIn(rightSide);
    }

    else
    {
      if(execvp(rightSide[0], rightSide) < 0)
      {
        printf("%s", "Error exec");
      }
    }

    execvp(rightSide[0], rightSide);
  }
}
