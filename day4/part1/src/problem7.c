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


#define MAX_MATCHES = 100;
int main(int argc, char *argv[])
{
    HashMap* winningValues = createHashMap(50);
    // input file
    FILE *inputFile = fopen("src/input.txt", "r");

    // making sure file opens
    if (inputFile == NULL) {
        perror("Error opening file");
        return 1;
    }

    // currentLine
    char *line = NULL;
    size_t sizeOfLine = 0;
    ssize_t statusOfRead;
    int sum = 0;

    // while there are lines to read
    while (( statusOfRead = getline(&line, &sizeOfLine, inputFile) ) != -1) {
        // extra data
        char* game = strtok(line, ":");
        // the game data
        char* gameData = strtok(NULL, ":");
        // the winning numbers
        char* winningNumbers = strtok(gameData, "|");
        // your numbers
        char* yourNumbers = strtok(NULL, "|");

        char* winningSavePtr = NULL;
        char* winningNum = strtok_r(winningNumbers, " ", &winningSavePtr);
        while( winningNum != NULL ) {
            put(winningValues, atoi(winningNum), 1);
            winningNum = strtok_r(NULL, " ", &winningSavePtr);
        }

        char* yourSavePtr = NULL;
        char* yourNum = strtok_r(yourNumbers, " ", &yourSavePtr);
        int score = 0;
        while( yourNum != NULL) {
            // if the winning number map contains your number then double the score
            if( get(winningValues, atoi(yourNum) ) != -1 ) {
                if( score == 0 ) {
                    score = 1;
                } else {
                    score *= 2;
                }
            }
            yourNum = strtok_r(NULL, " ", &yourSavePtr);
        }
        sum += score;
        // clear the winning numbers
        clearHashMap(winningValues);
    }
    printf("%i\n", sum);
}
