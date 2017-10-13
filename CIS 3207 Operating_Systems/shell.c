/*
Abstract of Approach
Techniques Used
Outline of Main Tasks of Each Process/Thread
Outline of Each Major Routine in Process/Thread
Input/Output of Each Function
**Include discussion of each possible function return value
*/
#include "builtin.h"

#define arrSize 1024
#define commandSize 7

char* readALine();
void printHostName();
int evaluate(char *line);
void parseLine(char *line, char* argv[arrSize]);
void forkProgram();
int runBuiltIn(char* argv[arrSize]);
void redirect(char* argv[arrSize], int flag, int i);
int isReDirect(char* argv[arrSize]);
int ispipe(char* argv[arrSize], char* leftSide[arrSize], char* rightSide[arrSize]);

int actualLength;


int main(int argc, char* argv[])
{
    if(argc > 1)
    {
        FILE *fp;
        char* arrayInput[arrSize];
        char *commandFile[] = {"batchfile.txt"};
        char input[arrSize];
        char *lineRead;

        fp = fopen(commandFile[0], "r");

        fgets(input, sizeof(input), fp);
        fclose(fp);
        lineRead = input;

        int b = evaluate(lineRead);
    }

    else
    {
        char *lineInput;
        system("clear");
        while(1)
        {
            printHostName();
            lineInput = readALine();
            if(strcmp(lineInput, "quit\n") == 0 || feof(stdin))
            {
                return EXIT_SUCCESS;
            }
            int a = evaluate(lineInput);
        }
    }

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

    int a = isReDirect(argv);

    if(a == 2) // pipe
    {
      return 1;
    }

    if(a == 0)
    {
        if(argv[0] == NULL)
        {
            return 0; //ignore empty lines
        }

        if (isBuiltIn(argv) == 1)
        {
            runBuiltIn(argv);
        }
        else
        {
            forkProgram();
            return 1;
        }
        return 1;
    }
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
    actualLength = counter-1;
}

int runBuiltIn(char* argv[arrSize])
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

void redirect(char* argv[arrSize], int flag, int i)
{
    int j = i;
    int in, out;
    pid_t pid;
    int savedin = dup(0);
    int savedout = dup(1);

    pid = fork();

    if(pid == 0)//child
    {

        if(flag == 1) // >
        {
            int output = open(argv[j + 1], O_WRONLY|O_CREAT,S_IRWXU|S_IRWXG|S_IRWXO);
            dup2(output, 1);
            close(output);
            argv[j] = NULL;
            argv[j+1] = NULL;
            ++j;
        }

        if(flag == 2) // <
        {
            int input = open(argv[j + 1], O_CREAT | O_RDONLY, 0666);
            dup2(input, 0);
            close(input);
            argv[j] = NULL;
            argv[j+1] = NULL;
            ++j;
        }

        if(flag == 3) // >>
        {
            int output = open(argv[j + 1], O_WRONLY | O_APPEND | O_CREAT, 0666);
            dup2(output, 1);
            close(output);
            argv[j] = NULL;
            argv[j+1] = NULL;
            ++j;
        }


        execvp(argv[0], argv); //////////////////////////
    }

    else if(pid > 0)//parent
    {
        waitpid(pid, NULL, WCONTINUED);
    }

    dup2(savedin, 0);
    close(savedin);
    dup2(savedout, 1);
    close(savedout);

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
        printf("%s", "Error: not a valid command");
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

int isReDirect(char* argv[arrSize])
{
    int redirected = 0;
    char* leftSide[arrSize];
    char* rightSide[arrSize];
    int i = 0;
    while(argv[i] != NULL)
    {
        if(ispipe(argv, leftSide, rightSide) == 1)
        {
          pipeEvaluate(leftSide, rightSide);
          return 2;
        }

        if(strcmp(argv[i], ">") == 0)
        {
            redirect(argv, 1, i);
            redirected = 1;
        }

        if(strcmp(argv[i], "<") == 0)
        {
            redirect(argv, 2, i);
            redirected = 1;
        }

        if(strcmp(argv[i], ">>") == 0)
        {
            redirect(argv, 3, i);
            redirected = 1;
        }

        i++;

    }
    return redirected;

}

int ispipe(char* argv[arrSize], char* leftSide[arrSize], char* rightSide[arrSize])
{
    int pipeNum = 0;

    int i = 0;
    int j, k;
    while(argv[i] != NULL)
    {
        if(strcmp(argv[i], "|") == 0)
        {
            for(j = 0; j < i; j++)
            {
                leftSide[j] = malloc(sizeof (char) * sizeof(argv[i]));
                char* strArg = argv[j];
                leftSide[j] = strArg;

            }

            int m = 0;
            for(k = i + 1; k < actualLength; k++)
            {
                rightSide[m] = malloc(sizeof (char) * sizeof(argv[k]));
                char* strArg = argv[k];
                rightSide[m] = strArg;
                m++;
            }
            pipeNum = 1;
        }
        i++;
    }
}
