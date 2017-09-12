//Thatyana Morales
//2-10-2017
//CIS2107
//Lab03
//Statement of Problem: ATM

#include <stdio.h>
#include <stdlib.h>

void myBalance(); //function prototype/declaration
void receipt();
unsigned int num; //the pin the user enters
unsigned int incorrectAttempt = 0; //keeps track of incorrect input
unsigned int answer = 0; //the option the user selects among the ATM choices
unsigned const int PIN = 3014; //Nana's PIN
unsigned int balance = 5000; //Nana's initial balance
unsigned int transactions = 0;
unsigned int withdrawalLimit = 0;
unsigned int depositLimit = 0;

int main(void)
{

	puts("\tWelcome to Your Personal ATM, Nana!"); //Welcome message
	printf("\tPlease enter your PIN number: "); //Asks user (Nana) for PIN number
	scanf("%u", &num); //scans PIN input
	while (num != PIN) //if the input does not match the PIN
	{
		++incorrectAttempt;
		if (incorrectAttempt == 3) //only 3 chances given to enter correct input.
		{
			puts("\tError: 3 unsuccessful attempts. Check yourself b4 u wreck yourself"); //terminates program and error message
			exit(0);
		}
		else
		{
			puts("\tIncorrect PIN number. Please try again");
			printf("\tPlease enter your PIN number: ");
			scanf("%u", &num);
		}
	}
do
	{
	printf("\n\tWhat would you like to do now?\n"); //list of available choices
	puts("\t1. Balance");
	puts("\t2. Cash Withdrawal");
	puts("\t3. Cash deposition");
	puts("\t4. Quit");

	printf("\tPlease enter the number: "); //asks user for choice selection
	scanf("%u", &answer);

	switch (answer) //action based on choice selected
	{
		case 1: //initiates balance function
		myBalance();
		break;

		case 2:
		withdrawal();
		break;

		case 3:
		deposit();
		break;

		case 4:
		puts("\tThank you for using this ATM!");
		printf("\n\tYour number of transactions is: %u\n", transactions);
		break;

		default:
		puts("\tError: not a choice");
		break;

	}
}
	while (answer != 4);
}

void myBalance() //function to print current balance
{
	printf("\n\tYour current balance is: $%d\n", balance);
	receipt();
}

int withdrawal()
{
	if (withdrawalLimit >= 1000)
	{
		puts("\tError: You have reached your daily withdrawal limit");
		return balance;
	}
	else
	{
		++transactions;
		int amount;
		puts("\tDaily Withdrawal Limit is $1000");
		printf("\tPlease enter your withdrawal amount: $");
		scanf("%d", &amount);

		if (amount <= 0)
		{
			puts("\tYou can't withdraw 0 or negative dollars. Please try again");
			return balance;
			}
		else if (amount > balance)
		{
			puts("\tYou cannot withdraw more than what you have!");
			return balance;
		}
		
		else
		{
			withdrawalLimit += amount;
			if (withdrawalLimit > 1000)
			{
				puts("\tError: this amount exceeds the daily withdrawal limit. Please try again.");
				withdrawalLimit -= amount;
				return balance;
			}
			while(amount % 20 != 0)
			{
				puts("\tSorry, you can only withdrawal in multiples of 20. We cannot output 1s, 5s, 10s, or coins. Please try again.");
				printf("\n\tPlease enter your withdrawal amount: $");
				scanf("%d", &amount);
			}
		}

		balance -= amount;

		receipt();
		return balance;
	}
}

int deposit()
{
	if (depositLimit >= 10000)
	{
		puts("\tError: You have reached your daily deposit limit.");
		return balance;
	}
	else
	{
		++transactions;
		int amount;
		puts("\tDaily deposit limit is $10,000.");
		printf("\n\tPlease enter your deposit amount: ");
		scanf("%u", &amount);

		if (amount <= 0)
		{
			puts("\tYou can't deposit 0 or negative dollars. Please try again");
			return balance;
		}
		withdrawalLimit += amount;
		if (withdrawalLimit > 10000)
		{
			puts("\tError: amount exceeds deposit limit. Please try again");
			withdrawalLimit -= amount;
			return balance;
		}

		balance += amount;
		receipt();
		return balance;
	}
}

void receipt()
{
	unsigned int rec;
	printf("\n\tWould you like a receipt? 1 = yes, 2 = no\n\t");
	scanf("%u", &rec);

	switch (rec)
	{
		case 2:
		puts("\tGreat, you're saving paper!");
		break;

		case 1:
		puts("\tPrinted!");
		break;

		default:
		puts("\tInvalid response, so I will not print it");
		break;
	}
}
