/*
Abstract of Approach
The purpose of this application is for the user to have created a file
which produces a series of 10 records that each consist of 120 random characters.
Afterwards, the said file is to have one random record selected, and compared
with what was written to the buffer originally. If the records are the same,
it prints "The records are the same", and if not, it prints "The records are
NOT the same". After these comparisons are made and written to a separate
comparisons text file, the original file in which the records are contained
is deleted. This program works in conjunction with either Timer1 or Timer2
(in that, application cannot run by itself.) In addition to this conjunction,
application logs the end times for either timer programs in its respected
files.

Techniques Used
I used two functions in addition to the main function to help me do this process.
Each of the two functions does a small part in helping the main function
complete the necessary tasks. Details on each function is written below.

Outline of Main Tasks of Each Process/Thread
Because this is the application program (the main program with which timer1 and
timer2 runs on), there are no processes/threads running in this instance.

Outline of Each Major Routine in Process/Thread
Again, application is the main program with which both timer programs operate
on, so discussions related to the processes is irrelevant.

Input/Output of Each Function
**Include discussion of each possible function return value
void recordsFile(FILE* fp, char records[10][120]);
This function creates the main file with which to write the records in. Not only
that, but it also generates random characters and writes them to the file for
each record. There is no return value for this function since it is a void
type function. The inputs are the name of the file that is to have the records,
and a buffer array which will hold the written records to be compared to later.

void compareRecords(FILE* fp, char records[10][120]);
This function takes the record file as input as well as the buffer array mentioned
earlier with the previous function I explained. It is with this function that
the random record number is chosen and read from the file to be compared to the
record that is in the buffer array that corresponds to that specified record.
Again, there is no return value since this is a void function. One interesting
thing to note is that, when comparing the functions, I used garbage read lines
since I used \n to separate the records for easy reading.
*/



#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <sys/time.h>


void recordsFile(FILE* fp, char records[10][120]); //Creates the initial file and writes 10 records of 120 random characters each.
void compareRecords(FILE* fp, char records[10][120]); //finds a specific record and compares it to what was written for that record.

int main(int argc, char* argv[]) //the arguments are taken from the timer1 and timer2 programs
{
  struct timeval end; //end time needed for timer1 and timer2
  gettimeofday(&end, NULL); //gets the time of day in relation to the current time, rather than clock

  srand(time(NULL)); //needed for the randomCharacterGenerator
  int i;

    FILE* initialFile; //file that contains the records
    FILE* recordReader; //file that reads the initialFile to compare a specific record

    if(argc > 1) //has to do with the arguments taken from timer1 and timer2. This isn't required since it's hard coded
    {
      initialFile = fopen(argv[1], "w"); //opens the necessary files from timer1 and timer2
    }

    else
    {
      printf("No arguments passed\n");
    }

    char records[10][120]; //the buffer array to keep all the records

    recordsFile(initialFile, records); //calls this function to create it and write the records
    fclose(initialFile);

    recordReader = fopen(argv[1], "r"); //opens the specified file to compare the records
    compareRecords(recordReader, records); //uses the recordReader to compare
    fclose(recordReader);



  int fileRemoved;
  fileRemoved = remove(argv[1]); //used to remove the file specified by timer1 and timer2.
  printf("%s", "file successfully removed");

   /*The following four statements are used to open
     the time file for the specific program, and
     write the end time to it before closing it.*/
   FILE* times;
   times = fopen(argv[2], "a+");
   fprintf(times, "%d\n", (int) end.tv_usec);
   fclose(times);

}

/*This is the function in which the file is created to hold all the records*/
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

/*This is the function that will use the newly created and written file to compare
  records read to and written in the buffer. */
void compareRecords(FILE* fp, char records[10][120])
{
  /*The following statements concern the results of comparing the records.
    The record that was written to the buffer as well as the record that was
    read are both written to the file along with the comparison statement*/
  FILE* comparisons;
  comparisons = fopen("comparisons.txt", "w"); //just to reset the file so there isn't trailing data from previous runs
  fclose(comparisons);
  comparisons = fopen("comparisons.txt", "a"); //begins appending from the newly created file

  char recordRead[120]; //array to read the record
  char garbage[10]; //collects any trailing newlines (\n)
  int i, j, k, length;
  int recordNumber;

  recordNumber = rand() % 10; //random record to read from the file
  fprintf(comparisons, "Record Number: %d\n", recordNumber); //prints the record number (0 initialized)

  char chosenRecord[120]; //holds the random record

  for (i = 0; i < 120; i++){
    chosenRecord[i] = records[recordNumber][i];
  }

  for(j = 0; j <=recordNumber; j++) //finds the record to read
  {
    fgets(recordRead, sizeof(recordRead), fp); //reads the actual record
    fgets(garbage, sizeof(garbage), fp); //collects the newline
    fgets(garbage, sizeof(garbage), fp); //collects the newline
  }

  length = strlen(recordRead);
  if (recordRead[length-1] == '\n') //removes the newline collected from fgets so it can be compared to what was written
  {
    recordRead[length-1] = '\0'; //replaces the string buffer with the string terminator
  }

  /*This prints both the read and written record to the comparison file along
    with the results.*/
  fprintf(comparisons, "Record Read: %s\n", recordRead); //prints the read record
  fprintf(comparisons, "Record Written: %s", ""); //this is actually written in the following for loop

  for(k = 0; k < length; k++){
    fprintf(comparisons, "%c", chosenRecord[k]); //prints the written record from the buffer in the previous statement
  }

  if (strncmp(chosenRecord, recordRead, 119) == 0){ //checks to see if what was read equals what was written
    fprintf(comparisons, "%s", "\nThe record comparisons are equal");
  }
  else{
    fprintf(comparisons, "%s", "\nThe record comparisons are NOT equal"); //only if the comparison is unsuccessful

  }

  fclose(comparisons); //closes the file after the comparisons are made. With each loop run, the comparisons are constantly replaced.

}
