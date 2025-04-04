#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define n 100
int row = 1, col = 1;

char instructions[n][n] = {"ldr", "str", "mov"};

typedef struct {
    int id;
    int row;
    int col;
    char* symbol;
    char* type;
}SymbolTable;

int s = 0;
SymbolTable sarr[n];



char advanceFilePointer(FILE* fptr, char c) {
    if(c == EOF){
        return EOF;
    }
    if (c == '\n') {
        row++;
        col = 1;
    }else if(c == '\t'){
        col += 4;
    } 
    else {
        col++;
    }

    return fgetc(fptr);
}

void getNextToken(FILE* fptr, char c){
    while(c != EOF){
        while(c == ' ' || c == '\t' || c == '\n'){
            c = advanceFilePointer(fptr, c);
        }

        if(c == '.'){
            while(c != EOF && c != '\t' && c != '\n' && c != ' '){
                c = advanceFilePointer(fptr, c);
            }
        }else{
            int i = 0;
            char symbol[n];
            char type[n] = "unknown";

            while(c != EOF && c != ' ' && c != '\t' && c != '\n'){
                symbol[i++] = c;
                c = advanceFilePointer(fptr, c);
            }
            symbol[i] = '\0';

            if(symbol[i - 1] == ':'){
                strcpy(type, "Label");
            }else if(symbol[0] == 'x'){
                strcpy(type, "Register");
            }else if(strcmp(symbol, "svc") == 0){
                strcpy(type, "Syscall");
            }else{
                for(int j = 0; j < 3; j++){
                    if(strcmp(symbol, instructions[j]) == 0){
                        strcpy(type, "instruction");
                    }
                }
            }

            if(strcmp(type, "unknown") != 0){
                sarr[s].id = s + 1;
                sarr[s].row = row;
                sarr[s].col = col;
                sarr[s].symbol = strdup(symbol);
                sarr[s].type = strdup(type);
                s++; 
            }
        }

        c = advanceFilePointer(fptr, c);
    }
}

void displaySymbolTable(SymbolTable s_arr[n]){
    printf("The Symbol Table for the given code is-> \n");
    
    for(int i = 0; i < s; i++){
        printf("ID : %d\n", s_arr[i].id);
        printf("Row : %d\n", s_arr[i].row);
        printf("Column : %d\n", s_arr[i].col);
        printf("Symbol: %s\n", s_arr[i].symbol);
        printf("Type : %s\n", s_arr[i].type);
        printf("\n");
    }
}

int main(){
    FILE* fptr;
    fptr = fopen("sample.txt", "r");

    char c = fgetc(fptr);
    getNextToken(fptr, c);
    displaySymbolTable(sarr);

    fclose(fptr);
    return 0;
}