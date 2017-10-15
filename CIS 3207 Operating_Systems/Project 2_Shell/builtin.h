/*
Abstract of Approach
This built in header takes care of all the built in commands supported
by this mini shell. When called, isBuiltIn tests whether or not the inputted
command is actually a built in, and directs the command to its appropriate
function. If not, it merely returns a false flag.

Techniques Used
There is one function that tests for built in, while the rest are one function
for each built in command. Although they are all ints, they do not return
much information besides the fact of whether it works or not. This header
also holds all of the packages that are to be involved in the shell.c file.

Outline of Main Tasks of Each Process/Thread
The only tasks in each function are the built in commands themselves. The
isBuiltIn command redirects the array to the appropriate function to be run,
and each of those functions run one specific command.

Outline of Each Major Routine in Process/Thread
The routine is as follows: isBuiltIn checks for built in command, and sends
the command to the appropriate function. Each function then has its own
process of running the command specified.

Input/Output of Each Function
**Include discussion of each possible function return value
Each function returns an int, but it is only to show that the operation was
successful. Most of the functions here accept the double char pointer as
an argument, and then run the command directly.


*/

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ioctl.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <dirent.h>


int shell_cd(char **args); //changes directory
int shell_clr(char **args); //clears the screen
int shell_dir(char **args); //displays all directories in the current directory
int shell_environ(char **args); //displays all environment variables
int shell_echo(char **args); //prints anything after the "echo" token
int shell_help(char **args); //prints the User Manual to the screen using the more filter (manually created)
int shell_pause(char **args); //pauses the shell until ONLY ENTER IS PRESSED.

int isBuiltIn(char* argv[]); //checks whether or not the command is built in
extern char **environ; //has exterior environment variables

char* builtin_command[] = //holds all the built in command names
{
  "cd",
  "clr",
  "dir",
  "environ",
  "echo",
  "help",
  "pause",
};

int (*builtin_function[])(char **) = //holds addresses to the functions
{
  &shell_cd, //changes directory function
  &shell_clr, //clears the screen function
  &shell_dir, //displays directories in current directory function
  &shell_environ, //displays environment variables function
  &shell_echo, //displays everything after the "echo" token function.
  &shell_help, //displays the User Manual using the manually created more filter function
  &shell_pause, //pauses the screen until the user presses enter function
};

/*This checks whether or not the entered command is a built in function. If
  so, it redirects to the appropriate function accordingly.*/
int isBuiltIn(char* argv[])
{
  int i; //cycles through list of built in commands
  for(i = 0; i < sizeof(builtin_command) / sizeof(builtin_command[0]); i++) //if the command matches a built in command
  {
    if(strcmp(argv[0], builtin_command[i]) == 0) //condition for a match
    {
      return 1; //sets flag to true
    }
  }
  return 0; //false flag
}

/*This function changes the current working directory. This also affects
  the printed hostname in the console.*/
int shell_cd(char **args)
{
  if(args[1] == NULL) //prints current directory if there are no other arguments
  {
    char cwd[1024]; //will hold name of directory
    getcwd(cwd, sizeof(cwd)); //gets directory
    printf("Current directory is: %s\n", cwd); //prints directory
  }
  else //more than one argument
  {
    if(chdir(args[1]) != 0) //if there is a change directory error
    {
      perror("cd error\n"); //handles error
    }
  }
}

/*This function clears the screen by measuring the window height, returning
  a bunch of newlines, then reverting back to the top of the screen*/
int shell_clr(char **args)
{
  int i; //cycles through rows
  struct winsize w; //has window dimensions
  ioctl(0, TIOCGWINSZ, &w); //holds window dimensions

  for(i = 0; i < w.ws_row; i++) //cycles through rows in screen
  {
    puts(""); //prints newlines
  }
  printf("\033[%dA", w.ws_row); //reverts back to beginning of the window
}

/*This function displays all the directories in the current directory*/
int shell_dir(char **args)
{
  DIR *directory; //directory variable
  struct dirent *reader; //reads from directory

  directory = opendir ("./"); //opens current directory
  if (directory != NULL) //as long as the directory isn't empty
   {
     while (reader = readdir (directory)) //reads through directories in cwd
       puts (reader->d_name); //prints directory on screen
     (void) closedir (directory); //closes directory variable
   }
  else //error with displaying directory
  {
   perror ("Error: Directory couldn't be opened"); //error handling for directory
  }
  return 0; //end of function
}

/*Displays all the environment variables*/
int shell_environ(char **args)
{
  int i = 1; //cycles through environment variables
  char *environmentVariable = *environ; //environment pointer

  for (; environmentVariable; i++) //endless cycle until it reaches null
  {
    printf("%s\n", environmentVariable); //prints environment variable
    environmentVariable = *(environ+i); //sets to next environment variable
  }
  return 0; //end of function
}

/*This function displays anything listed after the echo token*/
int shell_echo(char **args)
{
  int i = 1; //cycles through array strings after echo
  while(args[i] != NULL) //while there are strings left
  {
    printf("%s ", args[i]); //prints word
    i++; //increments counter
  }
  printf("%s", "\n"); //newline to separate next command prompt
}

/*This function displays the user manual using a manually created more
  filter, meaning that it'll print lines up to the end of the screen, then
  continues printing the next line when enter is pressed. This function
  exits if q is entered. */
int shell_help(char **args)
{
  int a = shell_clr(args); //clears screen for a fresh start
  int j = 0; //cycles through lines
  struct winsize w; //has dimensions of window
  ioctl(0, TIOCGWINSZ, &w); //holds dimensions of window

  char manualFile[1024]; //reads lines from file
    FILE *fp; //file pointer to open file
    fp=fopen("UserManual.txt", "r"); //opens user manual for read only
    while(fgets(manualFile, 1024, fp)!=NULL) //while not end of file
    {
        printf("%s", manualFile); // print the string
        j++; //increment counter

        if(j == w.ws_row-1) //if j reaches the end of the number of rows
        {
          char buffArray[10]; //checks for user input (either q or ENTER)
          while(fgets(manualFile, 1024, fp)!=NULL) //still not end of the file
          {
            fgets(buffArray, sizeof(buffArray), stdin); //user input is either ENTER or q
            if(strcmp((buffArray), "\n") == 0) //ENTER
            {
              char* token = strtok(manualFile, "\n"); //flushes out new line
              printf("%s", manualFile); //prints next line
            }
            if(strcmp((buffArray), "q\n") == 0) //if user enters q
            {
              fclose(fp); //closes file
              return 1; //exits function
            }
          }
          fclose(fp); //closes file
          return 1; //exits function
        }
    }
}

/*This function pauses the shell until the user presses ENTER only. For
  instance, it will not resume if characters are entered, only space*/
int shell_pause(char **args)
{
  char buffArray[500] = " "; //holds user input
  printf("Shell is paused. Press enter to continue\n"); //statement that alerts user the shell is paused

  while(strcmp((buffArray), "\n") != 0) //while the user did not press just ENTER
  {
    fgets(buffArray, sizeof(buffArray), stdin); //gets next user input
  }
  printf("Shell has resumed\n"); //once enter is pressed, the shell resumes
}
