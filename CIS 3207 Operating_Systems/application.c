/*
Abstract of Approach

Techniques Used

Outline of Main Tasks of Each Process/Thread

Outline of Each Major Routine in Process/Thread

Input/Output of Each Function
**Include discussion of each possible function return value*/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <sys/time.h>


void recordsFile(FILE* fp, char records[10][120]); //Creates the initial file and writes 10 records of 120 random characters each.
void compareRecords(FILE* fp, char records[10][120]); //finds a specific record and compares it to what was written for that record.

int main(int argc, char* argv[])
{
  srand(time(NULL));
  int i;
  struct timeval end;

  gettimeofday(&end, NULL);
  printf("End Time %s: %ld\n", argv[3], end.tv_usec);


  FILE* times;
  times = fopen(argv[2], "a+");
  fprintf(times, "%ld\n", end.tv_usec);
  int start = *argv[4];
  fprintf(times, "%ld\n", end.tv_usec - start);
  fclose(times);

  // FILE* timeDifference;
  // timeDifference = fopen(argv[2], "a+");
  // int startTime, endTime;
  // fscanf(timeDifference, "%d", &startTime);
  // fscanf(timeDifference, "%d", &endTime);
  // int difference = endTime - startTime;
  // fprintf(timeDifference, "\nDifference: %d\n", difference);
  // fclose(timeDifference);


    FILE* initialFile; //file that contains the records
    FILE* recordReader; //file that reads the initialFile to compare a specific record

    if(argc > 1)
    {
      initialFile = fopen(argv[1], "w");
    }

    else
    {
      printf("No arguments passed\n");
    }

    char records[10][120];

    recordsFile(initialFile, records);
    fclose(initialFile);

    recordReader = fopen(argv[1], "r");
    compareRecords(recordReader, records);
    fclose(recordReader);



  int fileRemoved;
  fileRemoved = remove(argv[1]);

   if(fileRemoved == 0)
   {
      printf("\n");
   }
   else
   {
      printf("...\n");
   }

}

void recordsFile(FILE* fp, char records[10][120])
{
  const char array[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789"; //array of characters to be used
  char randomCharacterGenerator;
  int i, j;

  for(i = 0; i < 10; i++) //loops through creating each of the 10 files.
  {
    for(j = 0; j < 120; j++) //loops through writing the 120 random characters for each record
    {
      randomCharacterGenerator = array[rand() % (sizeof(array)-1)]; //randomly takes a character
      fputc(randomCharacterGenerator, fp); //writes the random character to the record
      records[i][j] = randomCharacterGenerator;

    }
    fputs("\n\n", fp); //separates each record visually in the file
  }


}

void compareRecords(FILE* fp, char records[10][120])
{
  FILE* comparisons;
  comparisons = fopen("comparisons.txt", "w");
  char recordRead[120]; //array to read the record
  char garbage[10]; //collects any trailing newlines (\n)
  int i, j, k, length;
  int recordNumber;

  recordNumber = rand() % 10; //random record to read from the file
  fprintf(comparisons, "Record Number: %d\n", recordNumber); //prints the record number (0 initialized)

  char chosenRecord[120];

  for (i = 0; i < 120; i++){
    chosenRecord[i] = records[recordNumber][i];
  }

  for(j = 0; j <=recordNumber; j++) //finds the record to read
  {
    fgets(recordRead, sizeof(recordRead), fp);
    fgets(garbage, sizeof(garbage), fp); //collects the newline
    fgets(garbage, sizeof(garbage), fp); //collects the newline
  }

  length = strlen(recordRead);
  if (recordRead[length-1] == '\n') //removes the newline collected from fgets so it can be compared to what was written
  {
    recordRead[length-1] = '\0';
  }

  fprintf(comparisons, "Record Read: %s\n", recordRead);
  fprintf(comparisons, "Record Written: %s", "");

  for(k = 0; k < length; k++){
    fprintf(comparisons, "%c", chosenRecord[k]);
  }

  if (strncmp(chosenRecord, recordRead, 119) == 0){ //checks to see if what was read equals what was written
    fprintf(comparisons, "%s", "\nThe record comparisons are equal");
  }
  else{
    fprintf(comparisons, "%s", "\nThe record comparisons are NOT equal");

  }

  fclose(comparisons);

}
