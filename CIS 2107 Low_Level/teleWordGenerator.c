// Thatyana Morales
// 3-37-2017
// CIS 2107
// Lab08
// teleWordGenerator

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void  printWordsUtil(int number[], int curr_digit, char output[], int n, FILE *m);
void printWords(int number[], int n, FILE *m);

//Driver program
int main(void)
{
  FILE *m;
  m = fopen("file.txt", "w");


  int tele[7];
  int a, b, c, d, e, f, g;
  printf("Enter 7 numbers. For example: 2 3 4 5 6 7 8\n");
  scanf("%d %d %d %d %d %d %d", &a, &b, &c, &d, &e, &f, &g);
  tele[0] = a;
  tele[1] = b;
  tele[2] = c;
  tele[3] = d;
  tele[4] = e;
  tele[5] = f;
  tele[6] = g;


    int j = sizeof(tele) / sizeof(tele[0]);
    printWords(tele, j, m);

    fclose(m);
}

// hashTable[i] stores all characters that correspond to digit i in phone
const char hashTable[10][5] = {"", "", "ABC", "DEF", "GHI", "JKL",
                               "MNO", "PQRS", "TUV", "WXYZ"};

// A recursive function to print all possible words that can be obtained
// by input number[] of size n.  The output words are one by one stored
// in output[]
void  printWordsUtil(int number[], int curr_digit, char output[], int n, FILE *m)
{
    // Base case, if current output word is prepared
    int i;
    if (curr_digit == n)
    {
        fprintf(m, "%s\n", output);
        return ;
    }

    // Try all 3 possible characters for current digir in number[]
    // and recur for remaining digits
    for (i=0; i<strlen(hashTable[number[curr_digit]]); i++)
    {
        output[curr_digit] = hashTable[number[curr_digit]][i];
        printWordsUtil(number, curr_digit+1, output, n, m);
        if (number[curr_digit] == 0 || number[curr_digit] == 1)
            return;
    }
}

// A wrapper over printWordsUtil().  It creates an output array and
// calls printWordsUtil()
void printWords(int number[], int n, FILE *m)
{
    char result[n+1];
    result[n] ='\0';
    printWordsUtil(number, 0, result, n, m);
}
