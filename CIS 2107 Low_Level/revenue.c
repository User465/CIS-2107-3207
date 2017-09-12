//Thatyana Morales
//2-6-2017
//CIS2107
//Lab02: Paycheck & Revenue
//Statement of Problem: Revenue

#include <stdio.h>
#include <stdlib.h>

int main(void)
{
  float itemPrice;
  int quantity;
  float discount;

  puts("\tWelcome to \"Temple\" store");

  printf("\tEnter item price: ");
  scanf("%f", &itemPrice);

  if(itemPrice < 1)
  {
    puts("\tThis is not a valid item price.\n\tPlease run the program again");
    puts("\tThank You for using \"Temple\" store");
    exit(0);
  }

  printf("\tEnter quantity: ");
  scanf("%i", &quantity);

  if(quantity < 1)
  {
    puts("\tThis is not a valid quantity order.\n\tPlease run the program again");
    puts("\tThank You for using \"Temple\" store");
    exit(0);
  }
  else
  {
    if((1 <= quantity) && (quantity <=49))
    {
      discount = 0;
    }
    if((50 <= quantity) && (quantity <=99))
    {
      discount = 10;
    }
    if((100 <= quantity) && (quantity <=149))
    {
      discount = 15;
    }
    else
    discount = 25;
  }
  float cost = itemPrice * quantity;
  float discountAmount = (discount / 100) * cost;
  float total = cost - discountAmount;

  printf("\tThe item price is: $%.2f\n", itemPrice);
  printf("\tThe order is: %i item(s)\n", quantity);
  printf("\tThe cost is: $%.2f\n", cost);
  printf("\tThe discount is: %.2f%%\n", discount);
  printf("\tThe discount amount is: $%.2f\n", discountAmount);
  printf("\tThe total is: $%.2f\n", total);
  puts("\tThank You for using \"Temple\" store");



}
