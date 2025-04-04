#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define n 1000

typedef struct {
    int id;
    int row;
    int column;
    char *function;
    char *lexeme;
    char *type;
    int size;
}SymbolTable;

SymbolTable s_arr[n];

int rows = 0, cols = 0;
int c_id = 1;

char advanceFilePointer(FILE* fptr1, char c){
    if(c == EOF){
        return EOF;
    }
    if(c == '\n'){
        rows++;
        cols = 0;
    }
    else if(c == '\t'){
        cols += 4;
    }
    else cols++;

    return fgetc(fptr1);
}

void getNextToken(FILE* fptr1, char c){
    char keywords[10][10] = {"int", "char", "float", "double", "if", "do", "else", "for", "while"};

    while(c != EOF){
        while(c == ' ' || c == '\t' || c == '\n'){
            c = advanceFilePointer(fptr1, c);
        }
        if(c == '#'){
            while(c != EOF && c != '\n'){
                c = advanceFilePointer(fptr1, c);
            }
            c = advanceFilePointer(fptr1, c);
        }

        else if(c == '/'){
            c = advanceFilePointer(fptr1, c);
            if(c == '/'){
                while(c != EOF && c != '\n'){
                    c = advanceFilePointer(fptr1, c);
                }
                c = advanceFilePointer(fptr1, c);
            }
            else if(c == '*'){
                c = advanceFilePointer(fptr1, c);
                while(c != EOF && c != '*' && c != ' '){
                    c = advanceFilePointer(fptr1, c);
                }
                c = advanceFilePointer(fptr1, c);
                if(c == '/'){
                    c = advanceFilePointer(fptr1, c);
                }
            }
        }

        else {
            int i = 0;
            char temp1[n];
            char temp2[n];
            char *func;

            int start_col = cols;

            int keyword = 0, valid = 0;

            while (c != EOF && c != ' ' && c != '\t' && c != '\n' && i < n - 1) {
                temp1[i++] = c;
                c = advanceFilePointer(fptr1, c);
            }
            temp1[i] = '\0'; 

            for (int j = 0; j < 9; j++) {
                if (strcmp(temp1, keywords[j]) == 0) {
                    keyword = 1;
                    break;
                }
            }

            i = 0;
            if(keyword && c == ' '){
                c = advanceFilePointer(fptr1, c);
                while(c != EOF && c != ' ' && c != '\t' && c != '\n' && c != ';' && i < n - 1){
                    temp2[i++] = c;
                    c = advanceFilePointer(fptr1, c);
                }
                valid = 1;
                temp2[i] = '\0';
            }

            for(int i = 0; i < strlen(temp2); i++){
                if(temp2[i] == '('){
                    valid = 0;
                    func = strdup(temp2);
                    break;
                }
            }

            if(valid){
                s_arr[c_id].id = c_id;
                s_arr[c_id].row = rows;
                s_arr[c_id].column = start_col;
                s_arr[c_id].function = strdup(func);
                s_arr[c_id].lexeme = strdup(temp2);
                s_arr[c_id].type = strdup(temp1);
                if(strcmp(temp1, "int") == 0) s_arr[c_id].size = 4;
                else s_arr[c_id].size = 1;
                c_id++;
            }
        }

        c = advanceFilePointer(fptr1, c);
    }
}

void displaySymbolTable(SymbolTable s_arr[n]){
    printf("The Symbol Table for the given code is-> \n");
    
    for(int i = 1; i < c_id; i++){
        printf("ID : %d\n", s_arr[i].id);
        printf("Row : %d\n", s_arr[i].row);
        printf("Column : %d\n", s_arr[i].column);
        printf("Under The Function : %s\n", s_arr[i].function);
        printf("Identifier : %s\n", s_arr[i].lexeme);
        printf("Type : %s\n", s_arr[i].type);
        printf("Size : %d\n", s_arr[i].size);
        printf("\n");
    }
}

int main(){
    FILE* fptr1;
    fptr1 = fopen("sample.c", "r");
    if(fptr1 == NULL){
        printf("File Doesnt Exist\n");
    }

    char c = fgetc(fptr1);
    getNextToken(fptr1, c);
    displaySymbolTable(s_arr);

    fclose(fptr1);
    return 0;
}