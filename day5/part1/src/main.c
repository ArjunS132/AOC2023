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

// helper method to convert from map to map
HashMap* convert( HashMap *seeds, char* paragraph, int numSeeds ) {
    HashMap* converted = createHashMap(50);
    char* paragraphPtr = NULL;
    // get the title line out of the way
    printf("%s\n", strtok_r(paragraph, "\n", &paragraphPtr));
    char* line = strtok_r(NULL, "\n", &paragraphPtr);
    while( line != NULL ) {
        printf("%s\n", line);
        char* numberPointer = NULL;
        long long destination = atoll(strtok_r(trim(line), " ", &numberPointer));
        long long source = atoll(strtok_r(NULL, " ", &numberPointer));
        long long range = atoll(strtok_r(NULL, " ", &numberPointer));
        for (int i = 0; i < numSeeds; i++) {
            long long currSeed = get(seeds, i);
            if( currSeed >= source && currSeed <= source + range ) {
                put(converted, i, currSeed - source + destination );
                removePair(seeds, i);
            }
        }
        line = strtok_r(NULL, "\n", &paragraphPtr);
    }
    for( int i = 0; i < numSeeds; i++ ) {
        // if the current seed is not in the map
        if( get(converted, i) == -1 ) {
            put(converted, i, get(seeds, i) );
        }
    }
    // clearHashMap(seeds);
    return converted;
}

long long findMin( HashMap* map, int numSeeds) {
    long long min = get(map, 0);
    for (int i = 0; i < numSeeds; i++) {
        long long currNum = get(map, i);
        if( currNum < min ) {
            min = currNum;
        }
    }
    return min;
}

#define MAX_MATCHES = 100;
int main(int argc, char *argv[])
{
    HashMap* seeds = createHashMap(50);
    // input file
    FILE *inputFile = fopen("src/input.txt", "rb");

    // making sure file opens
    if (inputFile == NULL) {
        perror("Error opening file");
        return 1;
    }
    char *line = NULL;
    size_t sizeOfLine = 0;
    ssize_t statusOfRead;
    long long sum = 0;

    // while there are lines to read
    int gameNumber = 1;

    char* seedLine = NULL;
    // get the seeds line
    getline(&seedLine, &sizeOfLine, inputFile);
    seedLine = trim(seedLine);

    // parsing the seed line to get just the data I need
    strtok(seedLine, ":");
    char* seedData = strtok(NULL, ":");
    int numSeeds = 0;
    char* seed = strtok( trim(seedData), " " );



    while( seed != NULL ) {
        put(seeds, numSeeds, atoll(seed));
        seed = strtok( NULL, " " );
        numSeeds++;
    }


    char* currParagraph = malloc(0);
    // getting the extra new line that is there after reading
    getline(&line, &sizeOfLine, inputFile);
    while (( statusOfRead = getline(&line, &sizeOfLine, inputFile) ) != -1) {
        if(strcmp(line, "\n") == 0) {
            seeds = convert(seeds, currParagraph, numSeeds);
            // currParagraph = (char * ) realloc(currParagraph, 0);
        }
        else {
            currParagraph = (char * ) realloc(currParagraph, strlen(line) + strlen(currParagraph) );
            strcat(currParagraph, line);
        }
    }
    seeds = convert(seeds, currParagraph, numSeeds);
    printf("\nmin: %lli\n", findMin(seeds, numSeeds) );
}
