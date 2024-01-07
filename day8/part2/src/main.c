#include <stdio.h>
#include <stdlib.h>
#include <regex.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>
#include "HashMap.h"



// helper trim functions from stackoverflow
char *ltrim(char *s)
{
    while(isspace(*s)) s++;
    return s;
}

char *rtrim(char *s)
{
    char* back = s + strlen(s);
    while(isspace(*--back));
    *(back+1) = '\0';
    return s;
}

char *trim(char *s)
{
    return rtrim(ltrim(s));
}

typedef struct Instruction {
    char move;
    struct Instruction* next;
} Instruction;

void reverseString(char* str) {
    int length = strlen(str);
    int start = 0;
    int end = length - 1;

    while (start < end) {
        // Swap characters at start and end indices
        char temp = str[start];
        str[start] = str[end];
        str[end] = temp;

        // Move indices towards the center
        start++;
        end--;
    }
}

Instruction* parseInstructions(char* line) {
    Instruction* prevInstruction = NULL;
    line = trim(line);
    reverseString(line);
    for(int i = 0; i < strlen(line); i++ ) {
        char currMove = line[i];
        Instruction* currInstruction = (Instruction*) malloc(sizeof(Instruction));
        currInstruction->move = currMove;
        currInstruction->next = prevInstruction;
        prevInstruction = currInstruction;
    }
    Instruction* headNode = prevInstruction;
    while( prevInstruction->next != NULL ) {
         prevInstruction = prevInstruction->next;
    }
    prevInstruction->next = headNode;
    return headNode;
}

typedef struct Table{
    char* location;
    char* left;
    char* right;
    struct Table* next;
}Table;

Table* parseTable(FILE *inputFile) {
    Table* headNode = NULL;
    char *line = NULL;
    size_t sizeOfLine = 0;
    ssize_t statusOfRead;
    // read a line
    statusOfRead = getline(&line, &sizeOfLine, inputFile);
    while( statusOfRead != -1 ) {
        Table* currNode = (Table*) malloc(sizeof(Table));
        regex_t wordRegex;
        char wordPattern[] = "[A-z]+";
        int res;
        // compile regex
        regcomp(&wordRegex, wordPattern, REG_EXTENDED);
        regmatch_t wordMatch;
        // execute regex for locations in initial line
        int offset = 0;
        int termNum = 1;
        while (regexec(&wordRegex, line + offset, 1, &wordMatch, 0) == 0) {
            // Match found
            int start = wordMatch.rm_so + offset;
            int end = wordMatch.rm_eo + offset;

            char* word = (char*) malloc(3*sizeof(char));
            strncpy(word, line + start, end - start);

            offset = end;

            if (wordMatch.rm_eo - wordMatch.rm_so <= 1) {
                // Avoid infinite loop for zero-width matches
                offset++;
            }
            switch (termNum) {
                case 1: currNode->location = strdup(word);
                        break;
                case 2: currNode->left = strdup(word);
                        break;
                case 3: currNode->right = strdup(word);
                        break;
            }
            free(word);
            termNum++;
        }
        currNode->next = headNode;
        headNode = currNode;
        // regular expresion to get letters from a line
        statusOfRead = getline(&line, &sizeOfLine, inputFile);
    }

    return headNode;
}

// return null if not found, else returns the row without the next field
Table* searchRow(Table* table, char* term) {
    Table* copy = table;
    bool found = false;
    Table* returnExp = (Table*) malloc(sizeof(Table));
    while( copy != NULL && !found ) {
        if( strcmp(copy->location, term) == 0 ) {
            returnExp->location = copy->location;
            returnExp->left = copy->left;
            returnExp->right = copy->right;
        }
        copy = copy->next;
    }
    return returnExp;
}

Table* searchLastChar(Table* copy, char letter) {
    // Table* copy = table;
    bool found = false;
    Table* returnExp = NULL;
    while( copy != NULL && !found ) {
        if( copy->location[strlen(copy->location) - 1] == letter ) {
            returnExp = (Table*) malloc(sizeof(Table));
            returnExp->location = copy->location;
            returnExp->left = copy->left;
            returnExp->right = copy->right;
            returnExp->next = copy->next;
            found = true;
        }
        copy = copy->next;
    }
    return returnExp;
}

long long gcd(long long a, long long b) {
    while (b != 0) {
        long long temp = b;
        b = a % b;
        a = temp;
    }
    return a;
}

// Function to calculate the Lowest Common Multiple (LCM)
long long lcm(long long a, long long b) {
    // Use the formula: LCM(a, b) = |a * b| / GCD(a, b)
    return (a != 0 && b != 0) ? (a * b) / gcd(a, b) : 0;
}

int main(int argc, char *argv[])
{
    // input file
    FILE *inputFile = fopen("src/input.txt", "rb");
    HashMap* loopSizes = createHashMap(50);

    // making sure file opens
    if (inputFile == NULL) {
        perror("Error opening file");
        return 1;
    }

    // parse the hands into a list
    char *line = NULL;
    size_t sizeOfLine = 0;
    ssize_t statusOfRead;
    // read a line
    statusOfRead = getline(&line, &sizeOfLine, inputFile);
    // these are looped, so the last one points to the 1st one
    Instruction* originalInstruction = parseInstructions(line);

    // burn the dead line (has only a newline);
    statusOfRead = getline(&line, &sizeOfLine, inputFile);
    // table where I have location, left, and right
    Table* table = parseTable(inputFile);

    Table* currLoop = searchLastChar(table, 'A');
    int i = 0;
    int numLoops = 0;
    int count = 0;
    while(currLoop != NULL ) {
        // reverse the order
        int count = 0;
        char* currLocation = currLoop->location;
        Table* currPosition = searchRow(table, currLocation);
        Instruction* instruction = originalInstruction;
        while(currLocation[strlen(currLocation) - 1] != 'Z') {
            char move = instruction->move;
            if( move == 'R' ) {
                currPosition = searchRow(table, currPosition->right);
            } else if (move == 'L') {
                currPosition = searchRow(table, currPosition->left);
            }
            instruction = instruction->next;
            currLocation = currPosition->location;
            count++;
        }
        currLoop = searchLastChar(currLoop->next, 'A');
        put( loopSizes, i, count);
        i++;
        numLoops++;
    }
    long long answer = get(loopSizes, 0);
    for (int i = 1;i < numLoops;i++) {
        answer = lcm(answer, get(loopSizes, i) );
    }
    printf("%lli\n", answer);
}

