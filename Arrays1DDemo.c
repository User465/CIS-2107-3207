//Thatyana Morales
//2-22-2017
//CIS2107
//Lab 4
//Statement of Problem: Fun with 1D Arrays

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stddef.h>

void fillArray(int array[], unsigned int size);
void findWithRange(int array[], unsigned int size);
void reverseArray(int array[], unsigned int size);
void printArray(int array[], unsigned int size);
void reverseWithinRange(int array[], unsigned int size);
int findSequence(int array[], unsigned int size);

int main() //main function which will call all of the functions
{
	unsigned int size = 0;
	int find;
	while (size <= 0)
	{
	printf("\tEnter size of array: "); //asks user for size of array
	scanf("%u", &size);

	if (size <= 0)
	{
		puts("\tError: not a valid array size. Please try again");
	}
	}

	int myArray[size]; //makes the array
	fillArray(myArray, size);
	puts("");
	findWithRange(myArray, size);
	puts("");
	reverseArray(myArray, size);
	puts("");
	reverseWithinRange(myArray, size);
	puts("");
	find = findSequence(myArray, size);
	if (find == -1)
	{
		puts("\tSequence not found");
	}
	else
	{
		printf("\tSequence found at index %d\n", find);
	}
}

void fillArray(int array[], unsigned int size) //fills the array with random numbers
		{
	int LOW = 1;
	int HIGH = 0;

	while ((LOW > HIGH) || LOW < 0 || HIGH < 0 || HIGH > size || LOW > size) {
		printf("\tEnter your low: "); //asks the user for the LOW to HIGH range of values
		scanf("%u", &LOW);
		printf("\tEnter your high: ");
		scanf("%u", &HIGH);

		if (LOW > HIGH) {
			puts("\tError, Low > High. Try Again.\n");
		}
		if (LOW < 0 || HIGH <= 0 || HIGH > size || LOW > size)
		{
			puts("\tError: out of bounds. Please try again.");
		}
	}
	int i;
	int j;
	srand(time(NULL));
	for (i = 0; i < size; i++) {
		array[i] = LOW + rand() / (RAND_MAX / (HIGH - LOW + 1) + 1);

	}

printArray(array, size);
}

void findWithRange(int array[], unsigned int size) //finds max value within index range
{
	int LOW = 1;
	int HIGH = 0;
	int i;
	int max;

	while ((LOW > HIGH) || LOW < 0 || HIGH < 0 || HIGH > size || LOW > size) {
			printf("\tEnter your Low Range Index: "); //asks for specific range, continually asks if invalid input
			scanf("%u", &LOW);
			printf("\tEnter your High Range Index: ");
			scanf("%u", &HIGH);

			if (LOW > HIGH) {
				puts("\tError, Low > High. Try Again.");
			}
			if (LOW < 0 || HIGH <= 0 || HIGH > size || LOW > size)
			{
				puts("\tError: out of bounds. Please try again.");
			}
		}

	max = 0;

	   for (i = LOW; i <= HIGH; i++) //cycles through array range
	   {
		 if (max < array[i])
		 {
		    max = array[i];
		 }
	   }
	printf("\tMax = %d\n", max); //prints max after exiting for loop

}

void reverseArray(int array[], unsigned int size) //makes a copy of the original array and prints a reverse
{
	puts("\tHere is your reversed array: ");
	int array2[size];
	int index;
	int i;
	int j;

	for (index = 0; index < size; index++)
	{
		array2[index] = array[index]; //copies array elements into copy
	}

	for (i = 0; i < size/2; i++) {
		int temp = array2[size - i - 1];
		array2[size - i - 1] = array2[i];
		array2[i] = temp;
	}

printArray(array2, size);

}

void reverseWithinRange(int array[], unsigned int size) //reverses range of values in the original array
{
	int LOW = 1;
	int HIGH = 0;
	int i;
	int j;
	int index;
	int array2[size];

	while ((LOW > HIGH) || LOW < 0 || HIGH < 0 || HIGH > size || LOW > size) {
			printf("\tEnter your Reverse Low Range Index: "); //asks for specific range, continually asks if invalid input
			scanf("%u", &LOW);
			printf("\tEnter your Reverse High Range Index: ");
			scanf("%u", &HIGH);

			if (LOW > HIGH) {
				puts("\tError, Low > High. Try Again.\n");
			}
			if (LOW < 0 || HIGH <= 0 || HIGH > size || LOW > size)
			{
				puts("\tError: out of bounds. Please try again.");
			}
		}

		for (index = 0; index < size; index++)
		{
			array2[index] = array[index]; //copies array elements into second array
		}

		for (i = LOW; i < HIGH/2; i++) {
			int temp = array2[HIGH - i - 1];
			array2[HIGH - i - 1] = array2[i];
			array2[i] = temp;
		}

		puts("\tHere is your array with the range reversed: ");
		printArray(array2, size);

}

void printArray(int array[], unsigned int size)
{
	int i;
	for (i = 0; i < size; i++) { //prints the array
		if ((i % 10) == 0)
		{
			puts("");
		}
		printf("\t%d", array[i]);
	}
	puts("");
}

int findSequence(int array[], unsigned int size) //looks for sequence of numbers in array
{
	int x, y, i;
	puts("\tNow you will find the sequence. \n\tHere is your array printed again for your benefit");
	printArray(array, size);
	printf("\n\tEnter two numbers: ");
	scanf("%d %d", &x, &y);

	for (i = 0; i < size; i++)
	{
		if (array[i] == x)
		{
			if (array[i+1] == y)
			{
				return i;
			}
		}
	}
	return -1;
}
