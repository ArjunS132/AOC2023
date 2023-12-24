#include <stdio.h>
#include <stdlib.h>
#include <regex.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>
#include "HashMap.h"

// helper score calculation function (from part 1)
int calcScore(char *line) {
    HashMap* winningValues = createHashMap(50);

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
            score++;
        }
        yourNum = strtok_r(NULL, " ", &yourSavePtr);
    }


    clearHashMap(winningValues);
    // free(winningValues->array);
    // free(winningValues);
    return score;
}

#define MAX_MATCHES = 100;
int main(int argc, char *argv[])
{
    HashMap* gameWins = createHashMap(50);
    HashMap* gameCopies = createHashMap(50);
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
    long long sum = 0;

    // while there are lines to read
    int gameNumber = 1;
    while (( statusOfRead = getline(&line, &sizeOfLine, inputFile) ) != -1) {
        // put each games winning amount into a map
        int score = calcScore(line);
        put(gameWins, gameNumber, score);
        printf("line: %i", gameNumber);
        printf(" with score: %i\n", score);
        // put the current number of copies into a map
        put(gameCopies, gameNumber, 1);
        gameNumber++;
    }

    // loop through every game to start counting / making duplicates
    for (int i = 1; i < gameNumber; i++) {
        int dupeAmount = get(gameCopies, i);
        for( int j = 1; j + i < gameNumber && j <= get(gameWins, i); j++ ) {
            int currAmount = get(gameCopies, i + j);
            put(gameCopies, i + j, currAmount + dupeAmount );
        }
    }

    for( int i = 1; i < gameNumber; i++ ) {
        sum += get(gameCopies, i);
    }

    printf("%lli\n", sum);
}
