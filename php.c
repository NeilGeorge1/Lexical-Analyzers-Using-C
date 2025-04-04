#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX_SYMBOLS 100  
#define BUFFER_SIZE 100  

int i_d = 0;
int row = 1, col = 1;
char func[BUFFER_SIZE] = "global";

typedef struct {
    int id;
    int row;
    int col; 
    char lexeme[BUFFER_SIZE];
    char scope[BUFFER_SIZE];
    char type[BUFFER_SIZE];
} SymbolTable;

SymbolTable symbolTable[MAX_SYMBOLS];  
int symbolCount = 0;  

char advanceFilePointer(FILE* fptr, char c) {
    if (c == '\n') {
        row++;
        col = 1;
    } else {
        col++;
    }
    return getc(fptr);
}

void addToSymbolTable(char* lexeme, char* scope, char* type) {
    if (symbolCount >= MAX_SYMBOLS) {
        printf("Error: Symbol table full\n");
        return;
    }
    
    symbolTable[symbolCount].id = i_d++;
    symbolTable[symbolCount].row = row;
    symbolTable[symbolCount].col = col;
    strcpy(symbolTable[symbolCount].lexeme, lexeme);
    strcpy(symbolTable[symbolCount].scope, scope);
    strcpy(symbolTable[symbolCount].type, type);
    
    symbolCount++;
}

void getNextToken(FILE* fptr) {
    char c;
    while ((c = getc(fptr)) != EOF) {
        
        if (c == '<') {  
            while (c != EOF && c != ' ' && c != '\t' && c != '\n') {
                c = advanceFilePointer(fptr, c);
            }
        }

        else if (c == '/') {  
            c = advanceFilePointer(fptr, c);
            if (c == '/') {  
                while (c != '\n' && c != EOF) {
                    c = advanceFilePointer(fptr, c);
                }
            } else if (c == '*') {  
                c = advanceFilePointer(fptr, c);
                while (c != EOF) {  
                    if (c == '*' && (c = advanceFilePointer(fptr, c)) == '/') break;
                    c = advanceFilePointer(fptr, c);
                }
                c = advanceFilePointer(fptr, c);
            }
        }

        else if (c == '}') {
            strcpy(func, "global");
        }

        else {
            int i = 0, j = 0;
            char temp[BUFFER_SIZE], temp2[BUFFER_SIZE];

            while (c != ' ' && c != '\t' && c != '=' && c != ';' && c != '(' && c != ')' && c != EOF) {
                temp[i++] = c;
                c = advanceFilePointer(fptr, c);
            }
            temp[i] = '\0';

            if (temp[0] == '$') {
                addToSymbolTable(temp, func, "variable");
            }

            if (strcmp(temp, "function") == 0) {
                c = advanceFilePointer(fptr, c);
                while (c != '(') {
                    temp2[j++] = c;
                    c = advanceFilePointer(fptr, c);
                }
                temp2[j] = '\0';
                strcpy(func, temp2);
                addToSymbolTable(temp2, "global", "function");
            } else if (strcmp(temp, "define") == 0) {
                while (c != ' ' && c != '\t' && c != '\n' && c != EOF) {
                    c = advanceFilePointer(fptr, c);
                }
            }
        }
    }
}

void displaySymbolTable() {
    printf("\n%-5s %-15s %-10s %-10s %-10s\n", "ID", "Lexeme", "Row", "Column", "Type");
    printf("---------------------------------------------------\n");
    for (int i = 0; i < symbolCount; i++) {
        printf("%-5d %-15s %-10d %-10d %-10s\n",
               symbolTable[i].id,
               symbolTable[i].lexeme,
               symbolTable[i].row,
               symbolTable[i].col,
               symbolTable[i].type);
    }
}

int main() {
    FILE* fptr = fopen("sample.php", "r");
    if (!fptr) {
        printf("Error: Cannot open file\n");
        return 1;
    }

    getNextToken(fptr);
    fclose(fptr);

    displaySymbolTable();  

    return 0;
}
