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

typedef struct SeedRange {
    long long startValue;
    long long range;
    struct SeedRange* next;
} SeedRange;

typedef struct TableRow {
    long long destination;
    long long source;
    long long range;
    struct TableRow* next;
} TableRow;

TableRow* parseTable(FILE *inputFile) {
    char *line = NULL;
    size_t sizeOfLine = 0;
    ssize_t statusOfRead;
    // burn the title of the map
    statusOfRead = getline(&line, &sizeOfLine, inputFile);
    // get the first line to process
    statusOfRead = getline(&line, &sizeOfLine, inputFile);
    TableRow *tableHead = NULL;
    // while the line read isn't a newline
    while ( statusOfRead != -1 && strcmp(line, "\n") != 0) {
        char* numberPointer = NULL;
        long long destination = atoll(strtok_r(trim(line), " ", &numberPointer));
        long long source = atoll(strtok_r(NULL, " ", &numberPointer));
        long long range = atoll(strtok_r(NULL, " ", &numberPointer));
        TableRow* currRow = (TableRow*) malloc(sizeof(TableRow));
        currRow->range = range;
        currRow->source = source;
        currRow->destination = destination;
        currRow->next = tableHead;
        tableHead = currRow;
        // read the next line
        statusOfRead = getline(&line, &sizeOfLine, inputFile);
    }
    return tableHead;
}

void printTable(TableRow* table) {
    TableRow* copy = table;
    while( copy != NULL ) {
        printf("%lli ", copy->destination);
        printf("%lli ", copy->source);
        printf("%lli \n", copy->range);
        copy = copy->next;
    }
}

// given a line of seeds. parse them
SeedRange* parseSeeds(char* seedLine) {
    seedLine = trim(seedLine);

    // parsing the seed line to get just the data I need
    strtok(seedLine, ":");
    char* seedData = strtok(NULL, ":");
    char* seedsPtr = NULL;
    seedData = trim(seedData);
    char* seedStart = strtok_r( seedData, " ", &seedsPtr);
    char* seedlength = strtok_r( NULL, " ", &seedsPtr);
    SeedRange* seedHead = NULL;
    while( seedStart != NULL && seedlength != NULL ) {
        SeedRange* currSeed = (SeedRange*) malloc(sizeof(SeedRange));
        currSeed->startValue = atoll(seedStart);
        currSeed->range = atoll(seedlength);
        currSeed->next = seedHead;
        seedHead = currSeed;
        seedStart = strtok_r( NULL, " ", &seedsPtr);
        seedlength = strtok_r( NULL, " ", &seedsPtr );
    }
    return seedHead;
}

long long searchTable(TableRow* table, long long searchTerm) {
    long long res = -1;
    TableRow* copy = table;
    while( res == -1 && copy != NULL ) {
        if( searchTerm >= copy->destination && searchTerm <= copy->destination + copy->range ) {
            res = searchTerm - copy->destination + copy->source;
        }
        copy = copy->next;
    }
    return res == -1 ? searchTerm: res;
}

long long containedInInput(SeedRange* yourSeeds, long long searchSeed ) {
    long long res = -1;
    SeedRange* copy = yourSeeds;
    while( res == -1 && copy != NULL ) {
        if( searchSeed >= copy->startValue && searchSeed <= copy->startValue + copy->range ) {
            res = searchSeed - copy->startValue + copy->range;
        }
        copy = copy->next;
    }
    return res;
}

int main(int argc, char *argv[])
{
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

    char* seedLine = NULL;
    // get the seeds line
    getline(&seedLine, &sizeOfLine, inputFile);
    // parsed the seeds
    SeedRange* seedPairs = parseSeeds(seedLine);
    SeedRange* copyPairs = seedPairs;
    // skipping the newline that is below the seeds
    statusOfRead = getline(&line, &sizeOfLine, inputFile);
    TableRow* seedToSoil = parseTable(inputFile);
    TableRow* soilToFertilizer = parseTable(inputFile);
    TableRow* fertilizerToWater = parseTable(inputFile);
    TableRow* waterToLight = parseTable(inputFile);
    TableRow* lightToTemp = parseTable(inputFile);
    TableRow* tempToHumidity = parseTable(inputFile);
    TableRow* humidityToLocation = parseTable(inputFile);

    long long res = -1;
    long long location = 0;
    while( res == -1 ) {
        long long humidity = searchTable(humidityToLocation, location);
        long long temp = searchTable(tempToHumidity, humidity);
        long long light = searchTable(lightToTemp, temp);
        long long water = searchTable(waterToLight, light);
        long long fertilizer = searchTable(fertilizerToWater, water);
        long long soil = searchTable(soilToFertilizer, fertilizer);
        long long seed = searchTable(seedToSoil, soil);
        res = containedInInput(seedPairs, seed);
        if( res != -1 ) {
            res = location;
        }
        location++;
    }
    printf("location: %lli ", res);
}
