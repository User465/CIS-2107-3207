//Thatyana Morales
//2/26/2017
//CIS2107
//Lab5
//Statement of Problem: Arrays of Ptrs to Functions

// Fig. 6.22: fig06_22.c
// Two-dimensional array manipulations.
#include <stdio.h>
#define STUDENTS 3
#define EXAMS 4

// function prototypes
void minimum(int pupils, int tests, int grades[][EXAMS]);
void maximum(int pupils, int tests, int grades[][EXAMS]);
void average(int pupils, int tests, int grades[][EXAMS]);
void printArray(int pupils, int tests, int grades[][EXAMS]);
void (*processGrades[4]) (int pupils, int tests, int grades[][EXAMS]);

// function main begins program execution
int main(void)
{
  int choice = -1;
   // initialize student grades for three students (rows)
   int studentGrades[STUDENTS][EXAMS] =
      { { 77, 68, 86, 73 },
        { 96, 87, 89, 78 },
        { 70, 90, 86, 81 } };

    processGrades[0] = printArray;
    processGrades[1] = minimum;
    processGrades[2] = maximum;
    processGrades[3] = average;

    while(choice != 4)
    {
    puts("\n\n\t0  Print the array of grades");
    puts("\t1  Find the minimum grade");
    puts("\t2  Find the maximum grade");
    puts("\t3  Print the average on all tests for each student");
    puts("\t4  End Program");
    printf("\tEnter a choice: ");
    scanf("%d", &choice);


    switch (choice)
    {
      case 0:
      (*processGrades[choice])(STUDENTS, EXAMS, studentGrades);
      break;

      case 1:
      (*processGrades[choice])(STUDENTS, EXAMS, studentGrades);
      break;

      case 2:
      (*processGrades[choice])(STUDENTS, EXAMS, studentGrades);
      break;

      case 3:
      (*processGrades[choice])(STUDENTS, EXAMS, studentGrades);
      break;

      case 4:
      puts("\tOkay, bye.");
      break;

      default:
      puts("\tError: not a valid choice. Please try again.");
      break;

    }
  }
}

// Find the minimum grade
void minimum(int pupils, int tests,  int grades[][EXAMS])
{
   int lowGrade = 100; // initialize to highest possible grade
   int i, j;
   // loop through rows of grades
   for (i = 0; i < pupils; ++i) {

      // loop through columns of grades
      for (j = 0; j < tests; ++j) {

         if (grades[i][j] < lowGrade) {
            lowGrade = grades[i][j];
         }
      }
   }

   printf("\tThe minumum grade is: %d", lowGrade);
}

// Find the maximum grade
void maximum(int pupils, int tests,  int grades[][EXAMS])
{
   int highGrade = 0; // initialize to lowest possible grade
   int i, j;
   // loop through rows of grades
   for (i = 0; i < pupils; ++i) {

      // loop through columns of grades
      for (j = 0; j < tests; ++j) {

         if (grades[i][j] > highGrade) {
            highGrade = grades[i][j];
         }
      }
   }

   printf("\tThe maximum grade is: %d", highGrade);
}

// Determine the average grade for a particular student
void average(int pupils, int tests,  int grades[][EXAMS])
{
  int studentTotal = 0;
  int i = 0;
  int j = 0;

  for (i = 0; i < pupils; i++)
  {
    for(j = 0; j < tests; j++)
    {
      studentTotal += grades[i][j];
    }
    int average = (double) studentTotal / tests;
    printf("\tAverage of student %d is: %d\n", i+1, average);
    studentTotal = 0;
    average = 0;
  }
}

// Print the array
void printArray(int pupils, int tests,  int grades[][EXAMS])
{
  int i, j;
   // output column heads
   printf("%s", "                 [0]  [1]  [2]  [3]");

   // output grades in tabular format
   for (i = 0; i < pupils; ++i) {

      // output label for row
      printf("\nstudentGrades[%u] ", i);

      // output grades for one student
      for (j = 0; j < tests; ++j) {
         printf("%-5d", grades[i][j]);
      }
   }
}
