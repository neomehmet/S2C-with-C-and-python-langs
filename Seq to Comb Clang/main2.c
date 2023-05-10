
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
typedef struct fifo Fifo;
struct fifo{
	char *inputReg[20];
	char *outputReg[20];
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
	FILE *tempS2C = fopen("tempS2C.txt", "w");
	FILE *S2C= fopen("S2C.txt", "w");


	char *Line = (char*)malloc(300*sizeof(char)) ;
	char *Words = (char*)malloc(300*sizeof(char)) ;
	Fifo *Dff = (Fifo*)malloc(sizeof(Fifo));
	Fifo  *head, *iter; 	Dff->next = NULL; iter = Dff; 	head = Dff;

	int x;
	char *q;
	while( !feof(s27) ){
		Line = readLine(s27, Line);
		//printf("okunan-->%s\n", Line );
		 x = Sperate(Line);
		 if ( x == 1){ //x =1 ; bu register bulundu satiri
			/* iter =*/ fifoReg(Line,iter);
			 x=0;
		 }
		 else { //registersiz yazma 
		 	writeLine( tempS2C,Line);

		 }
	}
	fclose(tempS2C);
	tempS2C = fopen("tempS2C.txt", "r");


	while (!feof(tempS2C)){
		fgets(Line,300,tempS2C);
		printf("-----------------%s",Line);
			if ( strncmp(Line, "module", 6) == 0){
				for(int n=0; n<strlen(Line); n++){
					if(Line[n] != '(') {//Paranteze olan kadar kismi yazma
						fputc(Line[n],S2C);
					printf("%c",Line[n]);}

					else if (Line[n] == '('){ //parantez ve registerlardan gelenleri yazma
						fputc('(',S2C);
						while (head->next != NULL ){
							fputs(head->inputReg,S2C);
							fputc(',',S2C);
							fputs(head->outputReg,S2C);
							fputc(',',S2C);
							//printf(">>>>>>>>>>>>>>>>>>>%s   %s",head->inputReg, head->outputReg);
							head = head->next;

						}

					}

				}

			}
			else if(strncmp(Line, "input",5) == 0){

			}
			else if(strncmp(Line, "output",6) == 0){

			}
			else fputs(Line, S2C);


	}

	// debug
	while( Dff->next != NULL){
		printf("linked %s %s\n",Dff->outputReg, Dff->inputReg );
		Dff = Dff->next;
	}
	fclose(s27);
}

char* strReturn(char* str){
	return str;
}

void fifoReg(char* Line, Fifo *iter){
	while( iter->next != NULL)
		iter = iter->next;

	int i=0,index = 0 ;
	char *temp = (char*)malloc(sizeof(char)*20);

	for(int n=0; n<strlen(Line); n++){
		if (  Line[n] != ' ' && Line[n] != '(' && Line[n] != ')'&& Line[n] != ','
				&& Line[n] != ';' && Line[n] != '\0'){
					temp[i] = Line[n];
					i++;
				}
		else{
			//printf("size%d  strlen%d\n",sizeof(temp), strlen(temp) );
			//printf("temp temp temp%s index %d\n",temp,index );
			if ( index == 3) {
				strncpy(iter->outputReg,temp,sizeof(temp)) ;
				//printf("temp temp temp%s index %d ---- %s\n",temp, index, iter->outputReg);
			}
			if ( index == 4){
			 strncpy(iter->inputReg,temp,sizeof(temp))  ;
			 //printf("temp temp temp%s index %d ---- %s\n",temp, index, iter->inputReg);
		 	}
			for ( int q=0; q<sizeof(temp); q++) // temp icini bosaltiyor
				temp[q] = '\0';
			i=0;
			index++;
		}
	}
	printf("%s --- %s\n", iter->outputReg, iter->inputReg );
	iter->next = (Fifo*)malloc(sizeof(Fifo));
	iter = iter->next;
	iter->next = NULL;

//	return iter;
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
			for ( int q =0; q<sizeof(temp); q++){
				temp[q] = ' ';
			}
			i=0;

		}
	}
/*	if( write == 0){
		writeLine(target, Line);
	}
	 *///strcpy(Line, words);
	return write;
}
