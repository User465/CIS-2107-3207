#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <dirent.h>


int shell_cd(char **args);
int shell_clr(char **args);
int shell_dir(char **args);
int shell_environ(char **args);
int shell_echo(char **args);
int shell_help(char **args);
int shell_pause(char **args);

int isBuiltIn(char* argv[]);
extern char **environ;

char* builtin_command[] =
{
  "cd",
  "clr",
  "dir",
  "environ",
  "echo",
  "help",
  "pause",
};

int (*builtin_function[])(char **) =
{
  &shell_cd, //check
  &shell_clr, //check
  &shell_dir, //check
  &shell_environ, //check
  &shell_echo, //check
  &shell_help,
  &shell_pause, //check
};

int isBuiltIn(char* argv[])
{
  int i;
  for(i = 0; i < sizeof(builtin_command) / sizeof(builtin_command[0]); i++)
  {
    if(strcmp(argv[0], builtin_command[i]) == 0)
    {
      return 1;
    }
  }
  return 0;
}


int shell_cd(char **args)
{
  if(args[1] == NULL)
  {
    char cwd[1024];
    getcwd(cwd, sizeof(cwd));
    printf("Current directory is: %s\n", cwd);
  }
  else
  {
    if(chdir(args[1]) != 0)
    {
      perror("cd error\n");
    }
  }
}

int shell_clr(char **args)
{
  system("clear");
}

int shell_dir(char **args)
{
  DIR *directory;
  struct dirent *reader;

  directory = opendir ("./");
  if (directory != NULL)
   {
     while (reader = readdir (directory))
       puts (reader->d_name);
     (void) closedir (directory);
   }
  else
   perror ("Error: Directory couldn't be opened");

  return 0;
}

int shell_environ(char **args)
{
  int i = 1;
  char *s = *environ;

  for (; s; i++)
  {
    printf("%s\n", s);
    s = *(environ+i);
  }

  return 0;
}


int shell_echo(char **args)
{
  int i = 1;
  while(args[i] != NULL)
  {
    printf("%s ", args[i]);
    i++;
  }
  printf("%s", "\n");
}

int shell_help(char **args)
{
  // FILE *fp;
  // int a = shell_clr(args);
  // int c;
  // int n = 0;
  //
  // fp = fopen("UserManual.txt","r");
  // if(fp == NULL)
  // {
  //   perror("Error");
  //   return(0);
  // }
  // do
  // {
  //   c = fgetc(fp);
  //   if(feof(fp))
  //   {
  //      break ;
  //   }
  //   printf("%c", c);
  // }while(1);
  //
  // fclose(fp);
  // return(0);

  // int i;
  // printf("These are built in\n");
  //
  // for (i = 0; i < commandSize; i++) {
  //   printf("  %s\n", builtin_command[i]);
  }


int shell_pause(char **args)
{
  char buffArray[500] = " ";
  printf("Shell is paused. Press enter to continue\n");

  while(strcmp((buffArray), "\n") != 0)
  {
    fgets(buffArray, sizeof(buffArray), stdin);
  }
  printf("Shell has resumed\n");
}
