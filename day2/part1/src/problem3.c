#include <stdio.h>
#include <stdlib.h>
#include <regex.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>

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

    // initial game number and sum
    int gameNum = 0;
    int sum = 0;

    // while there are lines to read
    while (( statusOfRead = getline(&line, &sizeOfLine, inputFile) ) != -1) {
        gameNum++;
        bool goodGame = true;
        // wacky pointers for strtok_r
        char* setsSavePtr = NULL;
        char* colorSavePtr = NULL;
        // get the game num
        char* game = strtok(line, ":");
        // get the data for the game
        char* gameData = strtok(NULL, ":");
        // get all individual games. used r to allow looping
        char* sets = strtok_r(gameData, ";", &setsSavePtr);
        // loop through the sets
        while( sets != NULL ) {
            char* colorAndCount = strtok_r(sets, ",", &colorSavePtr);
            // loop through the colors and counts
            while( colorAndCount != NULL ) {
                char* trimmedStr = trim(colorAndCount);
                char* trimSavePtr = NULL;
                char* countAsStr = strtok_r(trimmedStr, " ", &trimSavePtr);
                // conver to an int
                int count = atoi( countAsStr );
                char* color = strtok_r(NULL, " ", &trimSavePtr);
                // check for valid games
                if( strcmp(color, "red") == 0 ) {
                    if( count > 12 ) {
                        goodGame = false;
                    }
                }
                if( strcmp(color, "green") == 0 ) {
                    if( count > 13 ) {
                        goodGame = false;
                    }
                }
                if( strcmp(color, "blue") == 0 ) {
                    if( count > 14 ) {
                        goodGame = false;
                    }
                }
                // get next count + color combo
                colorAndCount = strtok_r(NULL, ",", &colorSavePtr);
            }
            // get next set
            sets = strtok_r(NULL, ";", &setsSavePtr);
        }
        if( goodGame ) {
            sum += gameNum;
        }
    }
    printf("%i\n", sum);
    printf("\n");
}
