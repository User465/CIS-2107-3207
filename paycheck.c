//Thatyana Morales
//2-3-2017
//Lab02: Paycheck & Revenue
//Statement of Problem: Paycheck

#include <stdio.h>
#include <stdlib.h>


int main(void)
{
  int employeeNumber; //Employee's number
  float hourlySalary; //how much money per hour
  float weeklyTime; //How many hours a week they work
  float regularPay; //What they should get paid
  float overtimePay; //if they work overtime, how much money that would be
  float netPay; //overall payment

printf("Welcome to \"TEMPLE HUMAN RESOURCES\"\n\n"); //welcoming message

printf("\tEnter Employee Number: "); //asks user for employee number
scanf("%d", &employeeNumber); //takes input and saves into employeeNumber

if (employeeNumber < 0) //tests to see that the employeeNumber is valid******
{
  printf("\tThis is not a valid Employee Number.\n\tPlease run the program again\n");
  printf("\tThank you for using \"TEMPLE HUMAN RESOURCES\"\n");
  exit(0); //exists the program
}

printf("\tEnter Hourly Salary: "); //asks user how much money they get an hour
scanf("%f", &hourlySalary); //takes input and stores into hourlySalary

if (hourlySalary < 0) //tests validity of input
{
  printf("\tThis is not a valid hourly salary amount.\n\tPlease run the program again\n");
  printf("\tThank you for using \"TEMPLE HUMAN RESOURCES\"\n");
  exit(0);
}

printf("\tEnter Weekly Time: "); //asks user for weekly time
scanf("%f", &weeklyTime); //stores input into weeklyTime

if (weeklyTime < 0) //tests validity of input
{
  printf("\tThis is not a weekly time.\n\tPlease run the program again\n");
  printf("\tThank you for using \"TEMPLE HUMAN RESOURCES\"\n");
  exit(0);
}

printf("================================================\n"); //border

// printf("\tEmployee #: %d\n\tHourly Salary: %f\n", employeeNumber, hourlySalary);
printf("\tEmployee #: %d\n", employeeNumber);
printf("\tHourly Salary: %.2f\n", hourlySalary);
printf("\tWeekly Time: %.2f\n", weeklyTime);
regularPay = weeklyTime * hourlySalary;

printf("\tRegular Pay: $%.2f\n", regularPay);

if (weeklyTime <= 40)
{
  overtimePay = 0;
  netPay = regularPay;
}
else
{
  overtimePay = 1.5 * regularPay;
  netPay = (40 * regularPay) + ((weeklyTime - 40) * overtimePay);
}
printf("\tOvertime Pay: $%.2f\n", overtimePay);
printf("\tNet Pay: $%.2f\n", netPay);
printf("================================================\n");
printf("\tThank you for using \"TEMPLE HUMAN RESOURCES\"\n");


}
