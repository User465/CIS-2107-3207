/*
Abstract of Approach
For this project, I decided to split up the tasks in as
many functions as possible so one function isn't doing
so many calculations. I also tried to make each function
handle errors in their own ways so in case something
goes wrong in the output, I know where the error is coming
from.

Techniques Used
I split up the work into as many functions as possible so
each function does a small but very important role in the
grand scheme of things.

Outline of Main Tasks of Each Process/Thread
For each process that is forked, the parent process waits
for the child (if the background flag isn't set to true)
while the child either runs the built in function or execs
an outer function. No threads were created for this project,
but there is a separate fork function which handles these Tasks

Outline of Each Major Routine in Process/Thread
The routines for each process is as follows: check if the
input from the command line is a built in or not. If it is,
run the built in. If it is not, fork the process and allow
the child to run it. As for the pipes, the parent forks
twice to allow both the input of the pipe and the output
of the pipe to execute.

Input/Output of Each Function
**Include discussion of each possible function return value
Each function takes the original argv as input, along with
some other arguments as parameters. For instance, some
of the functions below accept an int isBackground, which
tells the function whether or not the process is to be run
in the background. For the others, such as ispipe and
pipeEvaluate, they accept a modified version of the original
command line input; they take the left side of the pipe as
an argument, and the right side of the pipe as another.
As for the returns, many of them are void so they return
nothing, whereas the rest mainly return an int which signals
whether or not to proceed with a specified action.
*/

#include "builtin.h" //this includes all the built in commands and related functions

#define arrSize 1024 //the size of the command line input
#define commandSize 7 //the number of built in arguments

char* readALine(); //takes the input directly from the command line
void printHostName(); //prints the shell name and current working directory
int evaluate(char *line); //takes in the raw input and evalutes what to do with it
void parseLine(char *line, char* argv[arrSize]); //used by evaluate, this parses the input into string tokens
void forkProgram(char* argv[arrSize], int isBackground); //as the name suggests, it forks a process to execute
int runBuiltIn(char* argv[arrSize]); //if it so happens the input is a built in, it runs the builtin.h
void redirect(char* argv[arrSize], int flag, int i); //coordinates the redirection process
int isReDirect(char* argv[arrSize], int isBackground); //checks whether the input contains a redirect symbol >, <, >>
int ispipe(char* argv[arrSize], char* leftSide[arrSize], char* rightSide[arrSize]); //checks whether the input contains a pipe
void pipeEvaluate(char* leftSide[arrSize], char* rightSide[arrSize], int isBackground); //called by ispipe, it handles running the piping process

int actualLength; //since the input buffer is 1024, this keeps track of the actual size of the input


/*The main function handles printing the cwd to the screen, taking input
  (or handling batchfile), and calling the right functions to evaluate.*/
int main(int argc, char* argv[])
{
    if(argc > 1) //the signal for batchfile
    {
        FILE *fp; //the file to open batchfile
        char *commandFile[] = {"batchfile.txt"}; //pointer to batchfile name
        char input[arrSize]; //grabs input from batchfile
        char *lineRead; //raw input read from commandFile

        fp = fopen(commandFile[0], "r"); //opens the batchfile for read only

        fgets(input, sizeof(input), fp); //reads line from file
        fclose(fp); //closes file
        lineRead = input; //sets input to line read
        int b = evaluate(lineRead); //passes line read through evaluate
    }

    else //no batchfile from command line
    {
        char *lineInput; //input to be read from command line
        system("clear"); //clears screen for fresh new start
        while(1) //endless loop, but ending condition is later on
        {
            printHostName(); //prints current working directory
            lineInput = readALine(); //reads raw input from user
            if(strcmp(lineInput, "quit\n") == 0 || feof(stdin)) //ending condition
            {
                return EXIT_SUCCESS; //quits shell
            }
            int a = evaluate(lineInput); //evaluates raw input
        }
    }
}

/*Reads raw input from user, and is used by evaluate function. It returns
  a char pointer to evaluate.*/
char *readALine()
{
    char input[arrSize]; //holds the input
    char *lineRead; //reads the input

    fgets(input, sizeof(input), stdin); //reads input and saves into input
    lineRead = input; //input saved into lineRead
    return lineRead; //char pointer that is returned in the function
}

/*Prints the current working directory to the screen. It does change and
  modify if the user enters cd*/
void printHostName()
{
    char hostname[arrSize]; //holds hostname
    char cwd[1024]; //holds working directory
    if (getcwd(cwd, sizeof(cwd)) != NULL) //makes sure nothing goes wrong!
    {
        gethostname(hostname, arrSize); //gets host name and saves into array
        printf("%s%s~>", hostname, cwd); //prints the hostname and cwd to the screen
    }
    else
        perror("getcwd() error\n"); //error if cwd does not work
}

/*This is the main function that handles the rest of the cases with input
  This is called by main function and takes the raw line as input. This
  parses the input accordingly and tests for each token instance. */
int evaluate(char *line)
{
    char* argv[arrSize]; //holds the input
    int isBackground = 0; //flag to check if the input contains a &

    parseLine(line, argv); //splits input into strings

    if(strcmp(argv[actualLength-1], "&") == 0) //checks whether & is in the input array
    {
      isBackground = 1; //sets flag to true if & is in the input
      argv[actualLength-1] = NULL; //deletes the & from the input so it doesn't interfere
    }

    int a = isReDirect(argv, isBackground); //calls isReDirect, which tests for I/O tokens

    if(a == 2) // if isReDirect returns the pipe flag
    {
        return 1; //exits since the pipe function handles its own functions
    }

    if(a == 0) //if there are no redirect tokens
    {
        if(argv[0] == NULL) //if there are no inputs
        {
            return 0; //ignore empty lines
        }

        if (isBuiltIn(argv) == 1) //if the command is a built in
        {
            runBuiltIn(argv); //runs builtin function
        }
        else //not a built in nor redirect
        {
            forkProgram(argv, isBackground); //if not a built in function, it execs the outside command
            return 1;
        }
        return 1;
    }
}

/*Parses the raw input and stores it in argv*/
void parseLine(char *line, char* argv[arrSize])
{
    char* token; //saves each string
    const char t[] = {" /\n"}; //separaters by which to divide the raw input
    int counter = 0; //to keep track of cycling through the input

    token = strtok(line, t); //saves the first work
    argv[counter] = token; //stores the first word into the array
    counter++; //increments counter

    while (token != NULL) //while there are still strings left in the raw input
    {
        token = strtok(NULL, t); //continues taking more strings
        argv[counter] = token; //continues saving into array
        counter++; //continues incrementing
    }
    actualLength = counter-1; //Once all this is over, the actual length of the array is stored
}

/*Called by isReDirect, this function takes care of running the built in
  functions that are called by the command line input*/
int runBuiltIn(char* argv[arrSize])
{
    int i; //counter
    for(i = 0; i < commandSize; i++) //cycles through list of built in functions
    {
        if(strcmp(argv[0], builtin_command[i]) == 0) //if there is a match
        {
            return (*builtin_function[i])(argv); //runs built in
        }
    }
    return 0; //function finished
}

/*Called by isReDirect, this function handles redirecting the command to the
  specified file (or getting input from the file) depending on the token*/
void redirect(char* argv[arrSize], int flag, int i)
{
    int j = i; //cycles through input starting from where the token of interest is
    int in, out; //will hold saved states of stdin and stdout
    pid_t pid; //forks process
    int savedin = dup(0); //saves the read side
    int savedout = dup(1); //saves the write side

    pid = fork(); //forks process into parent and child

    if(pid < 0) //error
    {
        printf("%s", "Error, can't fork\n"); //handles error
    }

    else if(pid == 0)//child process
    {
        if(flag == 1) // > is the redirect token
        {
            int output = open(argv[j + 1], O_WRONLY|O_CREAT,S_IRWXU|S_IRWXG|S_IRWXO); //opens the file for writing to or creating
            dup2(output, 1); //dups write side
            close(output); //closes stdout
            argv[j] = NULL; //sets > to null
            argv[j+1] = NULL; //sets file to null
            ++j; //increments
        }

        if(flag == 2) // < is the redirect token
        {
            int input = open(argv[j + 1], O_CREAT | O_RDONLY, 0666); //opens the file for reading
            dup2(input, 0); //dups read so it's from the input file
            close(input); //closes stdin
            argv[j] = NULL; //sets < to null
            argv[j+1] = NULL; //sets file name to null
            ++j; //increment counter
        }

        if(flag == 3) // >> is the redirect token
        {
            int output = open(argv[j + 1], O_WRONLY | O_APPEND | O_CREAT, 0666); //opens the file to be appended to
            dup2(output, 1); //dups writing
            close(output); //closes stdin
            argv[j] = NULL; //sets >> to null
            argv[j+1] = NULL; //sets file name to null
            ++j; //increments counter
        }
        execvp(argv[0], argv); //executes given commands
      }

    else if(pid > 0)//parent process
    {
        waitpid(pid, NULL, WCONTINUED); //waits for child process to finisg
    }

    dup2(savedin, 0); //restores read
    close(savedin); //closes savedin
    dup2(savedout, 1); //restores stdout
    close(savedout); //closes out
}

/*This function handles forking and executing a command if it is not built in
  It takes the array as input as well as a background flag*/
void forkProgram(char* argv[arrSize], int isBackground)
{
    pid_t pid; //pid used to for processes
    pid = fork(); //forks process

    if(pid < 0) //error
    {
        printf("%s", "Error, can't fork\n"); //handles error
    }

    else if(pid == 0) //child process
    {
        if(execvp(argv[0], argv) < 0) //checks for error in executing
        {
            printf("%s: Command not found\n", argv[0]); //handles error
            exit(0); //exits process
        }
        printf("%s", "Error: not a valid command\n"); //if the input is not even a command
    }

    else if(pid > 0 && isBackground == 0)//parent process and there is no background
    {
        int status; //wait status of parent
        if(waitpid(pid, &status, 0) < 0) //error in waiting
        {
            printf("%s", "ERROR\n"); //handles error in waitpid for parent
        }
    }
}

/*This is technically a boolean function, in that it signals whether or not to
  call the redirect function because there is a specified token in the array*/
int isReDirect(char* argv[arrSize], int isBackground)
{
    int redirected = 0; //flag set to true if there is an I/O token
    char* leftSide[arrSize]; //holds left side of the token
    char* rightSide[arrSize]; //holds the right side of the token
    int i = 0; //checks entire array for arrows
    while(argv[i] != NULL) //while there are strings left to check
    {
        if(ispipe(argv, leftSide, rightSide) == 1) //checks to see if there is a pipe in the array
        {
            pipeEvaluate(leftSide, rightSide, isBackground); //calls pipe function to handle the rest
            return 2; //returns pipe flag
        }

        if(strcmp(argv[i], ">") == 0) //if the found token is >
        {
            redirect(argv, 1, i); //redirects with the flag set to 1
            redirected = 1; //sets redirect flag to true
        }

        if(strcmp(argv[i], "<") == 0)
        {
            redirect(argv, 2, i); //redirects with the flag set to 2
            redirected = 1; //sets redirect flag to true
        }

        if(strcmp(argv[i], ">>") == 0)
        {
            redirect(argv, 3, i); //redirects with the flag set to 3
            redirected = 1; //sets redirect flag to true
        }

        i++; //increments counter

    }
    return redirected; //returns flag

}

/*This function checks whether or not the array contains a pipe. It takes in
  3 arrays, 1 of which is the original array and the other 2 will hold the left
  and right side of the pipe if it is contained. */
int ispipe(char* argv[arrSize], char* leftSide[arrSize], char* rightSide[arrSize])
{
    int pipeNum = 0; //flag to test if pipe is in it

    int i = 0; //counter to cycle through array
    int j, k; //cycles through left and right side to save strings into it
    while(argv[i] != NULL) //while array still has strings
    {
        if (strcmp(argv[i], "|") == 0) //if the pipe is there!
        {
            for (j = 0; j < i; j++) //cycles through strings on the left side of the pipe
            {
                leftSide[j] = malloc(sizeof(char) * sizeof(argv[i])); //allocates memory for specified array element
                char *strArg = argv[j]; //sets token into string
                leftSide[j] = strArg; //stores pointer into array
            }

            int m = 0; //specifically for right side array elements
            for (k = i + 1; k < actualLength; k++) //while we haven't reached the end of the array
            {
                rightSide[m] = malloc(sizeof(char) * sizeof(argv[k])); //allocates memory for specified array element
                char *strArg = argv[k]; //sets pointer to string
                rightSide[m] = strArg; //puts pointer in the array
                m++; //increments right side array counter
            }
            pipeNum = 1; //sets pipe flag to true
        }
        i++; //increments original array counter
    }
    return pipeNum; //returns flag
}

/*Called by ispipe, this function handles everything that is needed to successfully
  run the pipe command. It takes in the left and right sides of the original array
  and a background flag.*/
void pipeEvaluate(char* leftSide[arrSize], char* rightSide[arrSize], int isBackground)
{
    int fd[2]; //read and write end of the pipe
    pid_t pid; //for the first child process
    pid_t pid2; //for the second child process
    int savedin = dup(0); //saves state of read
    int savedout = dup(1); //saves state of write

    pipe(fd); //pipes read and write end
    pid = fork(); //forks process

    if(pid < 0) //error in forking
    {
        perror("Error forking\n"); //handles forking error
        exit(1);
    }

    if(pid == 0) //child process 1
    {
        dup2(fd[1], STDOUT_FILENO); //dups standard output
        close(fd[0]); //closes read side

        if(isBuiltIn(leftSide) == 1) //checks for built in all on its own
        {
            runBuiltIn(leftSide); //runs built in right into here
        }

        else //not a built in
        {
            if(execvp(leftSide[0], leftSide) < 0) //checks for error in executing
            {
                printf("%s", "Error exec\n"); //handles error
            }
        }
    }

    else //parent process
    {
        pid2 = fork(); //forks again into child process 2

        if(pid2 < 0) //error
        {
            perror("Error forking\n"); //handles error
            exit(1);
        }

        else if (pid2 == 0) //child process 2
        {
          dup2(fd[0], STDIN_FILENO); //dups read end
          close(fd[1]); //closes write end

          if(isBuiltIn(rightSide) == 1) //checks for built in once again
          {
              runBuiltIn(rightSide); //runs built in...once again
          }
          else //not a built in, my friend
          {
              if(execvp(rightSide[0], rightSide) < 0) // checks for executing errors
              {
                  printf("%s", "Error exec\n"); //execution handled ;)
              }
          }
        }

        else if(pid2 > 0 && isBackground == 0)//parent process yet again...this time with NO background!
        {
          close(fd[0]); //closes read
          close(fd[1]); //closes write
          waitpid(pid, NULL, 0); //waits for first child process to finish
          waitpid(pid2, NULL, 0); //waits for second child process to finish.
        }
    }
} //end of file
