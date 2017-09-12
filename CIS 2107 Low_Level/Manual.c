//Thatyana Morales
//03-20-2017
//CIS 2107
//Lab 7
//Statement of Problem: Manual

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
void upperLower(char array[]);
void convertStrtoInt(char a[], char b[], char c[], char d[]);
void convertStrtoFloat(char a[], char b[], char c[], char d[]);
void compareStr(char string1[], char string2[]);
void comparePartialStr(char string1[], char string2[], int compare);
void randomize();
void tokenizeTelNum(char telephone[]);
void reverse(char sentence[]);
void countSubstr(char *linesOfText, char *word);
void countChar(char *linesOfText, char letter);
void countAlpha(char *severalLines);
void countWords(char sentences[]);

int main()
{
	srand(time(NULL));

	upperLower("Hello!");

	char a[] = "289";
	char b[] = "143";
	char c[] = "222";
	char d[] = "333";
	convertStrtoInt(a, b, c, d);

	char e[] = "2.86";
	char f[] = "8.23";
	char g[] = "1.777";
	char h[] = "3.444";
	convertStrtoFloat(e, f, g, h);

	char stringa[100];
	char stringb[100];
	puts("Enter two strings: ");
	scanf("%s %s", stringa, stringb);
	compareStr(stringa, stringb);

	char stringc[100];
	char stringd[100];
	int input = 0;
	puts("Enter two strings and the # of characters to compare (enter a string, a string, and an int): ");
	scanf("%s %s %d", stringc, stringd, &input);
	comparePartialStr(stringc, stringd, input);

	randomize();
	puts("");

	char number[] = "(215) 555-8263.";
	tokenizeTelNum(number);
	puts("");

	char sent[] = "You are a cat";
	reverse(sent);

	char *line = "This is a cat and the cat is a hungry cat";
	char *cat = "cat";
	printf("Sentence: %s", line);
	countSubstr(line, cat);

	char *line2 = "My day today was a delicious day";
	char letter = 'd';
	printf("Sentence: %s", line2);
	countChar(line2, letter);
	puts("");
	puts("");

	char *lines = "the quick brown dirty ugly fox jumped over the lazy brown dog";
	countAlpha(lines);

	char wordy[] = "This is the longest sentence I hope to write in this entire code please give me a good grade";
	countWords(wordy);

}

//prints entered string as uppercase and lower case
void upperLower(char array[])
{
	char s[100];
	int i = 0;
	int j = 0;

		strcpy(s, array);

	while (s[i] != '\0')
	{
		printf("%c", (char) toupper(s[i]));
		i++;
	}
	puts("");

	while (s[j] != '\0')
	{
		printf("%c", (char) tolower(s[j]));
		j++;
	}
	puts("");

}

//converts 4 strings to integers, adds them, and prints the total.
void convertStrtoInt(char a[], char b[], char c[], char d[])
{
	int w = atoi(a);
	int x = atoi(b);
	int y = atoi(c);
	int z = atoi(d);
	printf("%d + %d + %d + %d = %d", w, x, y, z, w + x + y + z);
	puts("");
}

//converts 4 strings to floats, adds them, and prints the total.
void convertStrtoFloat(char a[], char b[], char c[], char d[])
{
	float w = atof(a);
	float x = atof(b);
	float y = atof(c);
	float z = atof(d);
	printf("%f + %f + %f + %f = %f", w, x, y, z, w + x + y + z);
	puts("");
}

//compares two strings to see which is greater, less, or if equal
void compareStr(char string1[], char string2[])
{

	int value = strcmp(string1, string2);

	if (value < 0)
	{
		puts("String 1 is less than String 2.");
	}
	else if (value > 0)
	{
		puts("String 1 is greater than String 2.");
	}
	else
	{
		puts("String 1 is equal to String 2.");
	}
}

//compares a portion of two given strings to see which is greater, less, or if equal
void comparePartialStr(char string1[], char string2[], int compare)
{

	int value = strncmp(string1, string2, compare);

	if (value < 0)
	{
		puts("String 1 is less than String 2.");
	}
	else if (value > 0)
	{
		puts("String 1 is greater than String 2.");
	}
	else
	{
		puts("String 1 is equal to String 2.");
	}

}

//generates random sentences
void randomize()
{
	int i;
	for (i = 0; i < 20; i++)
	{
	char sentence[200];
	char per[2] = ".";
	char* article[] = {" the", " a", " one", " some", " any"};
	char* noun[] = {" boy", " girl", " dog", " town", " car"};
	char* verb[] = {" drove", " jumped", " ran", " walked", " skipped"};
	char* preposition[] = {" to", " from", " over", " under", " on"};

	//first word article
	int a = rand() % 5;
	strcpy(sentence, article[a]);
	sentence[1] = toupper(sentence[1]);

	//noun
	a = rand() % 5;
	strcat(sentence, noun[a]);

	//verb
	a = rand() % 5;
	strcat(sentence, verb[a]);

	//preposition
	a = rand() % 5;
	strcat(sentence, preposition[a]);

	//article
	a = rand() % 5;
	strcat(sentence, article[a]);

	//noun
	a = rand() % 5;
	strcat(sentence, noun[a]);
	strcat(sentence, per);

	printf("%s", sentence);
	puts("");
	}
}

//splits the number into tokens, converts, and prints each.
void tokenizeTelNum(char telephone[])
{
	char *firstParenth;
	char *areaCode;
	char *number1;
	char *number2;


	//save area code as first token
	areaCode = strtok(telephone, "()");
	printf("Area Code: %s\n", areaCode);

	//save 3 digit number as 2nd token
	number1 = strtok(NULL, " -");
	printf("2nd Token String: %s\n", number1);

	//save 4 digit number as 3rd token
	number2 = strtok(NULL, ".");
	printf("3rd Token String: %s\n", number2);

	char sevenNum[8];
	strcpy(sevenNum, number1);
	strcat(sevenNum, number2);

	int areaCodeInt = atoi(areaCode);
	long int sevNum = atol(sevenNum);


	printf("Int Area Code: %d\nLong Int Number: %ld\n", areaCodeInt, sevNum);
}

//reverses a sentence
void reverse(char sentence[])
{
	printf("Original sentence: %s ", sentence);
	int i = 1;
	int j;
	char *token;
	char *words[4];

	//first word is tokenized
	token = strtok(sentence, " ");
	words[0] = token;

	while ((token = strtok(NULL, " ")) != NULL)
	{
		words[i] = token;
		i++;
	}

	printf("\nReversed sentence: ");

	for (j = 4; j >= 0; j--)
	{
		printf("%s ", words[j]);
	}

puts("");
puts("");
}

void countSubstr(char *linesOfText, char *word)
{
	int total = 0;
	while ((linesOfText = strstr(linesOfText, word)) != NULL)
	{
		total++;
		linesOfText++;
	}
	printf("\nNumber of occurrences of cat: %i", total);
	puts("");
	puts("");
}

//counts number of a character in a sentence
void countChar(char *linesOfText, char letter)
{
	int total = 0;
	while ((linesOfText = strchr(linesOfText, letter)) != NULL)
	{
		total++;
		linesOfText++;
	}
	printf("\t%c: %i", letter, total);
}

//counts all letters in a sentence.
void countAlpha(char *severalLines)
{
	printf("%s\n", severalLines);
	char a = 'a', b = 'b', c = 'c', d = 'd', e = 'e';
	char f = 'f', g = 'g', h = 'h', i = 'i', j = 'j';
	char k = 'k', l = 'l', m = 'm', n = 'n', o = 'o';
	char p = 'p', q = 'q', r = 'r', s = 's', t = 't';
	char u = 'u', v = 'v', w = 'w', x = 'x', y = 'y', z = 'z';

	countChar(severalLines, a);
	countChar(severalLines, b);
	countChar(severalLines, c);
	countChar(severalLines, d);
	countChar(severalLines, e);
	puts("");
	countChar(severalLines, f);
	countChar(severalLines, g);
	countChar(severalLines, h);
	countChar(severalLines, i);
	countChar(severalLines, j);
	puts("");
	countChar(severalLines, k);
	countChar(severalLines, l);
	countChar(severalLines, m);
	countChar(severalLines, n);
	countChar(severalLines, o);
	puts("");
	countChar(severalLines, p);
	countChar(severalLines, q);
	countChar(severalLines, r);
	countChar(severalLines, s);
	countChar(severalLines, t);
	puts("");
	countChar(severalLines, u);
	countChar(severalLines, v);
	countChar(severalLines, w);
	countChar(severalLines, x);
	countChar(severalLines, y);
	countChar(severalLines, z);
	puts("");
}

void countWords(char sentences[])
{
	printf("Original sentence: %s ", sentences);
	int total = 0;
	char *token = strtok(sentences, " ");
	total++;

	while ((token = strtok(NULL, " ")) != NULL)
	{
		total++;
	}

	printf("\nTotal number of words: %i", total);
	puts("");
	puts("");
}
