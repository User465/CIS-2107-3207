#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#define LENGTH 500

//function prototypes
void getQuote(char newLine[], char line[], int index[]);
int tokenize(char words[][LENGTH], char newLine[]);
int isVowel(char c);
void replace(char words[][LENGTH], char newLine[], int rows);
void lastIsVowel(char words[][LENGTH], char newLine[], int rows);

int main(void) {
    FILE* fp;
	FILE* fp2;
	int lineNum = 0;
	char words[50][LENGTH];
    char line[LENGTH];
	char temp[LENGTH];

	char newLine[LENGTH];

	int key = '"';
	char *p;

	int rows;
	int i; int j; int k;
	int len;

	int index[1];

    fp = fopen("sherlock.txt" , "r");
	fp2 = fopen("southie-sherlock.txt", "w");
	puts("opening file...");
    while (fgets(line, sizeof(line), fp) != NULL)
    {
        strcpy(temp,line);
		for(k=0; k<strlen(temp); k++){
		if(temp[k] == '"'){
			break;
		}
	}
		puts("writing to file...");
		p = strchr(line, key); //get line if it has a "
		if (p != NULL){ //if line has quotes do this

			strcpy(line, p); //copy contents of pointer into line array (seems to work?)
			len = strlen(line);

			getQuote(newLine, line, index);
			rows = tokenize(words, newLine);

			replace(words, newLine, rows);
			lastIsVowel(words, newLine, rows);

			if(k != 0){
				for(i=0; i<k; i++){
				fprintf(fp2, "%c", temp[i]);
				}
			}

			fprintf(fp2, "\"%s\"", newLine);
			for(i=index[1]+1; i<len; i++){
				fprintf(fp2, "%c", line[i]);
			}

		}
		else{
			fprintf(fp2, "%s", line);
		}

        //printf("%d %s", i, line);
		lineNum++;
    }
	puts("closing file...");
	fclose(fp);
	fclose(fp2);

}

//get dialogue only from line containing ""
void getQuote(char newLine[], char line[], int index[]){
	int i;
	int count = 0;
	//int index[1];
	int len;


	for(i=0; i<strlen(line)-1; i++){
		if(line[i] == '"'){
			index[count] = i;
			count++;
		}
	}

	//store quoted dialogue in new array
	for(i=index[0]+1; i<index[1]; i++){
		newLine[i-1] = line[i];
	}

	newLine[i-1] = '\0';

}

//separate dialogue into words into array
int tokenize(char words[][LENGTH], char newLine[]){
	int i = 0;

	char *p = strtok(newLine, " ");

	while (p != NULL){

		strcpy(words[i], p);
		p = strtok(NULL, " ");
		i++;
	}

	return i;

}

int isVowel(char c){
	switch(c){
		case 'A':
		case 'a':
		case 'E':
		case 'e':
		case 'I':
		case 'i':
		case 'O':
		case 'o':
		case 'U':
		case 'u':
			return 1;
		default:
			return 0;
	}
}

void replace(char words[][LENGTH], char newLine[], int rows){
	int i; int j; int k =0;
	int vowel;

	for(i=0; i<rows; i++){
			for(j=0; j<strlen(words[i]); j++){
			vowel = isVowel(words[i][j]);
			if(vowel == 1 && words[i][j+1] == 'r'){
				words[i][j+1] = 'h';
			}
			else if(words[i][j+1] == 'v' && words[i][j+2] == 'e'
			&& words[i][j+3] == 'r' && words[i][j+4] == 'y'){
				words[i][j+1] = 'w';
				words[i][j+2] = 'i';
				words[i][j+3] = 'c';
				words[i][j+4] = 'k';
				words[i][j+5] = 'e';
				words[i][j+6] = 'd';
			}
			//printf("%c", words[i][j]);
		newLine[k] = words[i][j];
		k++;

		}
		newLine[k++] = ' ';

	}
	newLine[k-1] = '\0';
	puts("");

}

//replace last letter of word that ends in a with r
void lastIsVowel(char words[][LENGTH], char newLine[], int rows){
	int i; int j; int k=0;
	int lastChar;

	for(i=0; i<rows; i++){
		lastChar = strlen(words[i]);

		//ignore single characters such as a
		if(lastChar > 0){
			if(words[i][lastChar] == 'a'){
			words[i][lastChar+1] = 'r';
			words[i][lastChar+1+1] = '\0';
			}
		}
		for(j=0; j<lastChar; j++){
			newLine[k] = words[i][j];
			k++;
		}
		newLine[k++] = ' ';
	}
	newLine[k-1] = '\0';
}
