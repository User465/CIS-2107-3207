#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>

#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <sys/time.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <sys/types.h>
#include <dirent.h>
#include "builtin.h"

#define BUF_SIZE 64
#define READ 0
#define WRITE 1
#define arrSize 1024
#define commandSize 7

int runBuiltIn(char* argv[]);
void pipeEvaluate(char* leftSide[arrSize], char* rightSide[arrSize]);
