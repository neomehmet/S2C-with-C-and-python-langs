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

char *readLineAndCheck( FILE*, char*);
char *prevSpaceHandling(char*);
int	Sperate( char*);
int whichPart(char*);
void fifoReg(char*, Fifo*);
void moduleDef(FILE* , char* , Fifo *);
void inputDef(FILE* , char* , Fifo *);
void outputDef(FILE* , char* , Fifo *);
void wireDef(FILE* , FILE* , char* , Fifo *);
int seekAndWrite(char* , Fifo*);

int main(int argc, char** argv){

	char *outputFile;
	char *inputFile;
	if ( argc < 3 || argc > 3){
		printf("hataaa\n");
	}
	else/* if ( argc == 3) */{ // argc == 2
		inputFile = argv[1];
		outputFile = argv[2];
		printf("input file name %s\noutputfile name %s\n",inputFile,outputFile);
	}
	char *tempFile = "tempS2C.txt" ;

	FILE *source = fopen(inputFile, "r");
	FILE *tempS2C = fopen(tempFile, "w");
	FILE *S2C= fopen(outputFile, "w");

	if (S2C == NULL) printf("DOSYAYI ACAMADIMMMM \n" );
	char *Line = (char*)malloc(300*sizeof(char)) ;

	Fifo *Dff = (Fifo*)malloc(sizeof(Fifo));
	Fifo  *head, *iter; 	Dff->next = NULL; iter = Dff; 	head = Dff;

	int x;
	char *q;
	while( !feof(source) ){
		Line = readLineAndCheck(source, Line);
		 x = Sperate(Line);
		 if ( x == 1){ //x =1 ; bu register bulundu satiri
			 fifoReg(Line,iter);
			 x=0;
		 }else { //registersiz yazma

		 		fputs(Line,tempS2C);
		 		}
	}
	fclose(tempS2C);
	tempS2C = fopen(tempFile, "r");
	iter = head;
	while (!feof(tempS2C)){
		fgets(Line,300,tempS2C);
		if ( strncmp(Line, "module", 6) == 0){
			moduleDef(S2C,Line,iter);
		}else if(strncmp(Line, "input",5) == 0){
			inputDef(S2C,Line,iter);
		}else if(strncmp(Line, "output",6) == 0){
			outputDef(S2C,Line,iter);
		}else if(strncmp(Line, "wire", 4) == 0){
			iter = head;
			wireDef(S2C,tempS2C,Line,iter);
		}else { fputs(Line, S2C);
			if ( ( strncmp(Line, "endmodule",9) == 0) )
			break;
		}
	}

//remove("tempS2C.txt");
	fclose(tempS2C);
	fclose(source);
	fclose(S2C);
	free(Line);
}  // main end // 


//*******************************
/* function bodies are defined  below */
//*******************************//
//module dff (CK,Q,D); Q OUT D input
void moduleDef(FILE* dest, char* Line, Fifo *head){
	Fifo* iter = head;
	int n;
	for(int n=0; n<strlen(Line); n++){
		if(Line[n] != '(') //Paranteze olan kadar kismi yazma
			fputc(Line[n],dest);
		else if (Line[n] == '('){ //parantez ve registerlardan gelenleri yazma
		//	fputc('\t',dest);
			fputc(Line[n],dest);
			int q=0;
			while (iter->next != NULL ){
				fputs(iter->inputReg,dest);
				fputc(',',dest);
				fputs(iter->outputReg,dest);
				fputc(',',dest);
				q += 2+ strlen(iter->inputReg) + strlen(iter->outputReg);
				iter = iter->next;
				if ( q >80){
					 fputc('\n',dest);
					 q =0;
				 }
				}

			}

		}
}

void inputDef(FILE* dest, char* Line, Fifo *head){
	Fifo* iter = head;
	int n=0;
	fputc('\n',dest);
	for(int n=0; n<strlen(Line); n++){
		if(Line[n] != ' ') // bosluga kadar kismi yazma
			fputc(Line[n],dest);
		else if (Line[n] == ' '){ // bsoluk ve registerlardan gelenleri yazma
			fputc(' ',dest);
			int q=0;
			while (iter->next != NULL ){
				fputs(iter->inputReg,dest);
				fputc(',',dest);	q += 2+ strlen(iter->inputReg) + strlen(iter->outputReg);
					iter = iter->next;
					if ( q >80){
						 fputc('\n',dest);
				//		 fputc('\t',dest);
						 q =0;
					 }
				}
			}
	}
}

void outputDef(FILE* dest, char* Line, Fifo *head){
	Fifo* iter = head;
	int n=0;
	fputc('\n',dest);
	for(int n=0; n<strlen(Line); n++){
		if(Line[n] != ' ')
			fputc(Line[n],dest);
		else if (Line[n] == ' '){
			fputc(' ',dest);
			int q=0;
			while (iter->next != NULL ){
				fputs(iter->outputReg,dest);
				fputc(',',dest);	q += 2+ strlen(iter->outputReg) + strlen(iter->outputReg);
					iter = iter->next;
					if ( q >80){
						 fputc('\n',dest);
				//		 fputc('\t',dest);
						 q =0;
					 }
				 }
			}
	}
}

void fifoReg(char* Line, Fifo *head){
	Fifo* iter = head;
	int i=0,index = 0 ;
	char *temp = (char*)malloc(sizeof(char)*20);
	while( iter->next != NULL)
		iter = iter->next;

	for(int n=0; n<strlen(Line); n++){
		if (  Line[n] != ' ' && Line[n] != '(' && Line[n] != ')'&& Line[n] != ','
				&& Line[n] != ';' && Line[n] != '\0'){
					temp[i] = Line[n];
					i++;
					}
		else{
			if ( index == 3)
				strncpy(iter->outputReg,temp,strlen(temp)) ;
			if ( index == 4)
				strncpy(iter->inputReg,temp,strlen(temp))  ;
			for ( int q=0; q<sizeof(temp); q++) // temp icini bosaltiyor
				temp[q] = '\0';

			i=0;
			index++;
			}
	}

	iter->next = (Fifo*)malloc(sizeof(Fifo));
	iter = iter->next;
	iter->next = NULL;
}

char* prevSpaceHandling(char* line){ // satir basindaki bosluklari silme
	int i=0;
	while(1){
		if ( line[i] == ' '){ // bosluk oldugunda indexi ilerletiyor atama yapmiyor
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

char* readLineAndCheck(FILE* file, char* Line){ // satir satir okurken satir baslarinda
	if ( feof(file) ){ return NULL; }   // bosluk olanlarin bosluklarini siliyor
	int run = 1;
	char* line = (char*)malloc(300);
	fgets(line,300,file);
	while(run){
		if ( line[0] == ' ' )
			line = prevSpaceHandling(line);// satir basi boslugu silen kod
		else
			run = 0;
		}
	return line;
}

int whichPart(char* str){ // string == dff ? 1:0;
	if ( strcmp(str, "dff") == 0)
		return 1;
	else return 0;
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
		}else{
			run = ( strcmp(temp, "dff") == 0) ; // debug		printf("%d%s\n",run,temp);
			if (run == 1 )
				write = 1; //dff bulundu
			for ( int q =0; q<strlen(temp); q++)
				temp[q] = ' ';
			i=0;
		}
	}
	return write;
}

void wireDef(FILE* dest, FILE* src, char* Line, Fifo *head){
	Fifo* iter = head;
	char str[20] = {};
	int n, k=0, write=0;
	char tempC;

	for ( n=0; n<strlen(Line); n++){
		tempC = Line[n];
		fputc(Line[n],dest);
		if( tempC == ' ' )
			break;
		}

	for(int i = n+1; i<strlen(Line); ){
		if(Line[i] != ',' && Line[i] != ';' ){
			str[k] = Line[i];
			// debugging printf("------>%c ---line-->%c\n", str[k],Line[i]);
			k++;
			}
		else if ( Line[i] == ',' || Line[i] == ';') {
			// debugging printf("s ---%s---\n",str);
			iter = head;
			str[k+1] = '\0';
			write = seekAndWrite(str,iter);
			char c = ',', c1=';';
			if( write == 0){
				fputs(str,dest);
				if(Line[i] == ';')
					fputc(';',dest);
				else
					fputc(c,dest);
			}
			for ( k =0;k<sizeof(str); k++)	str[k] = '\0'; //str temizleme
				//str[k-1]= '\0';
			k=0;
		}

		if( i == strlen(Line)-1 ){ //satir sonunu kontrolu
			fgets(Line,300,src);
			for ( k = 0; k<sizeof(str); k++) str[k] = '\0';
				//str[k-1]= '\0';
			i = 0; // satir basindan tekrar baslamak icin
			k = 0; // str yi 0. indexten tekrar doldurmak icin
			}
		else i++;
		if(Line[i-1] == ';') { 	fputc('\n', dest); return;} //wire bitmis mi ?
	}
}

int seekAndWrite(char* str, Fifo* head){
	Fifo *iter = head;
	int write =0;
	while(iter->next != NULL){
		int ar;
	 // debugging   if( strcmp(str, "g18 ") == 0) { // debugging   printf("str=<%s>  input=<%s>   output=<%s>\n",str,iter->inputReg,iter->outputReg ); scanf("%d",&ar);}
	//printf("str=<%s>  input=<%s>   output=<%s>\n",str,iter->inputReg,iter->outputReg );
		if( (strcmp(str, iter->inputReg) == 0) )
			write +=1;
		if ( (strcmp(str, iter->outputReg) == 0))
			write +=1;
		iter = iter->next;
		}
	return write; //  = 0 sa YAZ, degilse yazma
}
