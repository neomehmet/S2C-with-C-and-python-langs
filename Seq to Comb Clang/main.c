#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
typedef struct fifo Fifo;
struct fifo{
	char *inputReg;
	char *outputReg;
	struct fifo* next;
};
char	*readLine( FILE*, char*);
char	*prevSpaceHandling(char*);
void writeLine(FILE*, char *);
int	Sperate( char *);
int whichPart(char* );
void fifoReg(char*, Fifo*);
char* outputFinder(char*);
char* inputFinder(char* Line);

int main(){
	FILE *s27 = fopen("s27.txt", "r");
	FILE *s2c = fopen("s2c.txt", "w");

	char *Line = (char*)malloc(300*sizeof(char)) ;
	char *Words = (char*)malloc(300*sizeof(char)) ;
	Fifo *Dff = (Fifo*)malloc(sizeof(Fifo));
	Fifo  *head, *iter; 	Dff->next = NULL; iter = Dff; 	head = Dff;

	int x;
	char *q;
	while( !feof(s27) ){
		Line = readLine(s27, Line);
		printf("okunan-->%s\n", Line );
		 x = Sperate(Line);
		 if ( x == 1){
			 iter->outputReg = outputFinder(Line);
			 iter->inputReg = inputFinder(Line);
			 printf("linked %s------%s \n",head->outputReg, iter->inputReg );
			 while( iter->next != NULL)
				 iter = iter->next;
			 iter->next = (Fifo*)malloc(sizeof(Fifo));
			 iter = iter->next;
			 iter->next = NULL;
		 }

		//printf("ayirma-<%s",Line );
	}
	while( head->next != NULL){
		printf("linked %s %s\n",head->outputReg, head->inputReg );
		head = head->next;
	}
	fclose(s2c);
	fclose(s27);
}

char* outputFinder(char* Line){
	char *temp = (char*)malloc(sizeof(char)*20);
	int i=0,index =0 ;
	for(int n=0; n<strlen(Line); n++){
		if (  Line[n] != ' ' && Line[n] != '(' && Line[n] != ')'&& Line[n] != ','
				&& Line[n] != ';' && Line[n] != '\0'){
					temp[i] = Line[n];
					i++;
				}
		else{
			if ( index == 3){
				printf("----->%s output index %d\n",temp,index );
				return temp ;
			}
			i=0;
			index++;
			for ( int q =0; q<20; q++){
				temp[q] = ' ';
			}
		}
	}
	return NULL;
}
char* inputFinder(char* Line){
	char *temp = (char*)malloc(sizeof(char)*20);
	int i=0,index =0 ;
	for(int n=0; n<strlen(Line); n++){
		if (  Line[n] != ' ' && Line[n] != '(' && Line[n] != ')'&& Line[n] != ','
				&& Line[n] != ';' && Line[n] != '\0'){
					temp[i] = Line[n];
					i++;
				}
		else{
			if ( index == 4){
			//	printf("----->%s output index %d\n",temp,index );
				return temp ;
			}
			i=0;
			index++;
			for ( int q =0; q<20; q++){
				temp[q] = ' ';
			}
		}
	}
	return NULL;
}

Fifo* fifoReg(char* Line, Fifo *iter){
	Fifo* fftemp = (Fifo*)malloc(sizeof(Fifo));
	fftemp->next = NULL;
	int i=0,index = 0 ;
	char *temp = (char*)malloc(sizeof(char)*20);
	while( iter->next != NULL)
		iter = iter->next;

	iter->next = (Fifo*)malloc(sizeof(Fifo));
	iter = iter->next;
	iter->next = NULL;

	for(int n=0; n<strlen(Line); n++){
		if (  Line[n] != ' ' && Line[n] != '(' && Line[n] != ')'&& Line[n] != ','
				&& Line[n] != ';' && Line[n] != '\0'){
					temp[i] = Line[n];
					i++;
				}
		else{
			printf("%s index %d\n",temp,index );
			if ( index == 3) fftemp->outputReg = temp ;
			if ( index == 4) fftemp->inputReg = temp  ;
			i=0;
			index++;
			for ( int q =0; q<20; q++){
				temp[q] = ' ';
			}
		}
	}
	printf("%s ---%s\n", fftemp->outputReg, fftemp->inputReg );
	iter->next = fftemp;
	return iter;
}
//*******************************//
/* function bodies are defined  below */
//*******************************//
//module dff (CK,Q,D); Q OUT D input
char* prevSpaceHandling(char* line){ // satir basindaki bosluklari silme
	char* newLine = (char*)malloc(sizeof(char)*strlen(line));
	int i=0;
	while(1){
		if ( line[i] == ' '){
			i++;
		}
		if( line[i] != ' ')
		break;
	}
	char* temp = (char*)malloc(sizeof(char)*(300));
	for( int j=0; i<strlen(line); j++){
		temp[j] = line[i];
		i++;
	}
	return temp;
}

char* readLine(FILE* file, char* Line){
	if ( feof(file) ){ return NULL; }
	int run = 1;
	char* line = (char*)malloc(300);
	fgets(line,300,file);
	while(run){
		if ( line[0] == ' ' )
			line = prevSpaceHandling(line);
		else
			run = 0;
	}
//	printf("okunan->%s", line );
	return line;
}
int whichPart(char* str){
// str == dff ? 1:0;
	if ( strcmp(str, "dff") == 0)
		return 1;
	else return 0;
}

void writeLine(FILE* file, char *Line){
	fputs(Line,file);
}

int	Sperate( char *Line){

	char *temp = (char*)malloc(sizeof(char)*20);
	char *words = (char*)malloc(sizeof(char)*300);
	int i = 0, run = 0, write = 0; //write permssion to writeLine()

	for(int n=0; n<strlen(Line); n++){
		if (  Line[n] != ' ' && Line[n] != '(' && Line[n] != ')'&& Line[n] != ','
				&& Line[n] != ';' && Line[n] != '\0'){
					temp[i] = Line[n];
					i++;
				}
		else{
			run = whichPart(temp); // debug		printf("%d%s\n",run,temp);
			if (run == 1 )
				write = 1; //dff bulundu
			for ( int q =0; q<20; q++){
				temp[q] = ' ';
			}
			i=0;
		}
	}
	return write;
}
