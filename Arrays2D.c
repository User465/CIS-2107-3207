//Thatyana Morales
//2/26/2017
//CIS2107
//Lab5
//Statement of Problem: 2D Arrays Processing

#include <stdio.h>
#include <stdbool.h>

void max(int rows, int columns, int array[][columns]);
void rowSum(int rows, int columns, int array[rows][columns]);
void columnSum(int rows, int columns, int array[rows][columns]);
bool isSquare(int rows, int columns);
void displayOutputs(int rows, int columns, int array[rows][columns]);

int main()
{
  int rows = 0;
  int columns = 0;
  int i = 0;
  int j = 0;

  while (rows <= 0 || columns <=0)
  {
  puts("\tLet's create a 2Dim array!");
  printf("\t\tHow many rows? ");
  scanf("%d", &rows);
  printf("\t\tHow many columns? ");
  scanf("%d", &columns);
  if (columns <= 0 || rows <= 0)
  {
    puts("Error: invalid input. Try again.");
  }
  }

int myArray[rows][columns];
int entry = 0;

for (i = 0; i < rows; i++)
{
  for (j = 0; j < columns; j++)
  {
    printf("\t\tenter [%d][%d]: ", i, j);
    scanf("%d", &myArray[i][j]);
  }
}

max(rows, columns, myArray);
rowSum(rows, columns, myArray);
columnSum(rows, columns, myArray);
bool x = isSquare(rows, columns);
if (x == true)
{
  puts("\tThis is a square");
}
else
{
  puts("\tThis is not a square");
}
displayOutputs(rows, columns, myArray);
}

void max(int rows, int columns, int array[][columns])
{
  int maximum = array[0][0];
  int i = 0;
  int j = 0;

  for (i = 0; i < rows; i++)
  {
    for (j = 0; j < columns; j++)
    {
      if (array[i][j] > maximum)
      {
        maximum = array[i][j];
      }
    }
  }
  printf("\tThe maximum value in the array is: %d\n", maximum);
}

void rowSum(int rows, int columns, int array[rows][columns])
{
  int rowTotal = 0;
  int i = 0;
  int j = 0;

  for (i = 0; i < rows; i++)
  {
    for(j = 0; j < columns; j++)
    {
      rowTotal += array[i][j];
    }
    printf("\tSum of row %d is: %d\n", i+1, rowTotal);
    rowTotal = 0;
  }
}

void columnSum(int rows, int columns, int array[rows][columns])
{
  int columnTotal = 0;
  int i = 0;
  int j = 0;

  for (j = 0; j < columns; j++)
  {
    for(i = 0; i < rows; i++)
    {
      columnTotal += array[i][j];
    }
    printf("\tSum of column %d is: %d\n", j+1, columnTotal);
    columnTotal = 0;
  }
}

bool isSquare(int rows, int columns)
{
  return (rows == columns);
}

void displayOutputs(int rows, int columns, int array[][columns])
{
  int i = 0;
  int j = 0;
  puts("\tHere is your 2Dim array:");
  for (i = 0; i < rows; i++)
  {
    printf("\t[");
    for (j = 0; j < columns; j++)
    {
      if(j+1 == columns)
      {
        printf("%d", array[i][j]);
      }
      else
      {
        printf("%d, ", array[i][j]);
      }
    }
    printf("]\n");
  }
}
