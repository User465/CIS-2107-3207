//Thatyana Morales
//3-6-2017
//CIS2107
//Lab06
//Statement of problem: Race

#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

enum Status{WON, CONTINUE};
int turtlePosition();
int harePosition();
void printArray(char *nPtr);/////////////
void movetPointer(char array[], char *tPtr, int track);
void movehPointer(char array[], char *hPtr, int track);
void resetArray(char array[]);
bool behindArray(int track, int move);

int main()
{
  srand(time(NULL));//////////
  rand();///////////
  puts("ON YOUR MARK, GET SET\nBANG!!!!!\nAND THEY'RE OFF!!!!!");//////
  int i = 0;/////
  int ttrack = 0;
  int htrack = 0;
  int tmove = 0;
  int hmove = 0;
  char position[70];

  while (i < 70)
  {
    position[i] = ' ';
    i++;
  }

  char *tPtr = position;
  char *hPtr = position;
  enum Status game = CONTINUE;



while (game == CONTINUE)
{
  printArray(position);
  resetArray(position);

  tmove = turtlePosition();
  hmove = harePosition();


  if (behindArray(ttrack, tmove) == true) //if the movement goes behind the array for turtle
  {
    tPtr = position;
    tmove = 0;
  }

  if (behindArray(htrack, hmove) == true) //if the movement goes behind the array for hare
  {
    hPtr = position;
    hmove = 0;
  }

  else if ((ttrack + tmove) >= 69) //if turtle goes past finish line
  {
    if ((htrack + hmove) >= 69) //if hare also goes past finish line
    {
      puts("Tie");
      game = WON;
    }

    else
    {
      puts("Turtle wins! Yay!");
      game = WON;
    }
  }

  else if ((htrack + hmove) >= 69) //if only the hare goes past the finish line
  {
    puts("Hare wins. Yuch");
    game = WON;
  }


  else if ((ttrack + tmove) == (htrack + hmove))
  {
    resetArray(position);
    if ((ttrack + tmove) >= 64)
    {
      puts("OUCH!!");
      ttrack += tmove;
      htrack += hmove;
    }
    else
    {
    position[ttrack + tmove] = 'O';
    position[ttrack + tmove + 1] = 'U';
    position[ttrack + tmove + 2] = 'C';
    position[ttrack + tmove + 3] = 'H';
    position[ttrack + tmove + 4] = '!';
    position[ttrack + tmove + 5] = '!';
    ttrack += tmove;
    htrack += hmove;
  }
  }

  else
  {
    ttrack += tmove;
    htrack += hmove;
    movetPointer(position, tPtr, ttrack);
    movehPointer(position, hPtr, htrack);
  }

}


}

int turtlePosition()
{

  int i = (rand() % 10) + 1; //range from 1-10 (rand() % (max +1 - minimum) + minimum;

  if (1 <= i && i <= 5) //fast plod
  {
    return 3;
  }

  else if (6 <= i && i <= 7) //slip
  {
    return -6;
  }

  else //slow plod
  {
    return 1;
  }

  return 0;
}

void movetPointer(char array[], char *tPtr, int track)
{
  *tPtr = ' ';
  tPtr = &array[track];
  *tPtr = 'T';
}

int harePosition()
{
  int i = (rand() % 10) + 1; //range from 1-10 (rand() % (max +1 - minimum) + minimum;

  if (1 <= i && i <= 2) //sleep
  {
    return 0;
  }

  else if (3 <= i && i <= 4) //big hop
  {
    return 9;
  }

  else if (i == 5) //big slip
  {
    return -12;
  }

  else if (6 <= i && i <=8) //small hop
  {
    return 1;
  }

  else
  {
    return -2;
  }
  return 0;

}

void movehPointer(char array[], char *hPtr, int track)
{
  *hPtr = ' ';
  hPtr = &array[track];
  *hPtr = 'H';
}

void printArray(char *nPtr)
{
  printf("%s\n", nPtr);
}

void resetArray(char array[])
{
  int i = 0;
  while (i < 70)
  {
    array[i] = ' ';
    i++;
  } ////
}

bool behindArray(int track, int move)
{
  return ((track + move) < 0);
}
