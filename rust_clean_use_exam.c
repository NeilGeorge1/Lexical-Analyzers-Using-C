#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#define n 1000

typedef struct {
    int id;
    int row;
    int column;
    char* symbol;
    char *type;
    int size;
}SymbolTable;

SymbolTable s_arr[n];

int rows = 1, cols = 1;
int c_id = 1;

char advanceFilePointer(FILE* fptr1, char c){
    if(c == EOF){
        return EOF;
    }
    if(c == '\n'){
        rows++;
        cols = 1;
    }
    else if(c == '\t'){
        cols += 4;
    }
    else cols++;

    return fgetc(fptr1);
}

void getNextToken(FILE* fptr1, char c){
    char keywords[10][10] = {"fn", "let", "struct"};

    while(c != EOF){
        while(c == ' ' || c == '\t' || c == '\n'){
            c = advanceFilePointer(fptr1, c);
        }

        int i = 0;
        char symbol[n];
        char type[n] = "unknown";

        int start_col = cols;

        int keyword = 0, valid = 0;

        while (c != EOF && c != ' ' && c != '\t' && c != '\n' && c != '(' && i < n - 1) {
            symbol[i++] = c;
            c = advanceFilePointer(fptr1, c);
        }
        symbol[i] = '\0'; 

        for (int j = 0; j < 3; j++) {
            if (strcmp(symbol, keywords[j]) == 0) {
                strcpy(type, "Keyword");
                break;
            }
        }

        if(c == '('){
            strcpy(type, "Function");
        }

        if(symbol[i - 1] == '!'){
            strcpy(type, "Macro");
        }else if(symbol[i - 1] == ':'){
            symbol[i - 1] = '\0';
            strcpy(type, "Varible");
        }else if(isdigit(symbol[i - 1])){
            strcpy(type, "Datatype");
        }

        if(strcmp(type, "unknown") != 0){
            s_arr[c_id].id = c_id;
            s_arr[c_id].row = rows;
            s_arr[c_id].column = start_col;
            s_arr[c_id].symbol = strdup(symbol);
            s_arr[c_id].type = strdup(type);
            c_id++;
        }

        c = advanceFilePointer(fptr1, c);
    }
}

void displaySymbolTable(SymbolTable s_arr[n]) {
    printf("\n%-5s %-6s %-8s %-25s %-15s\n", "ID", "Row", "Column", "Symbol", "Type");
    printf("-----------------------------------------------------------------------\n");

    for (int i = 1; i < c_id; i++) {
        printf("%-5d %-6d %-8d %-25s %-15s\n", 
               s_arr[i].id, 
               s_arr[i].row, 
               s_arr[i].column, 
               s_arr[i].symbol, 
               s_arr[i].type);
    }
}

int main(){
    FILE* fptr1;
    fptr1 = fopen("sample.rs", "r");

    char c = fgetc(fptr1);
    getNextToken(fptr1, c);
    displaySymbolTable(s_arr);

    fclose(fptr1);
    return 0;
}