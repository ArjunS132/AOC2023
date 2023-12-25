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

int main(int argc, char *argv[])
{
    HashMap* times = createHashMap(50);
    HashMap* distances = createHashMap(50);
    // input file
    FILE *inputFile = fopen("src/input.txt", "rb");

    // making sure file opens
    if (inputFile == NULL) {
        perror("Error opening file");
        return 1;
    }
    size_t sizeOfLine = 0;
    ssize_t statusOfRead;
    long long sum = 0;


    char* line = NULL;
    // get the time line
    getline(&line, &sizeOfLine, inputFile);

    // parsing the seed line to get just the data I need
    strtok(line, ":");
    char* timeData = strtok(NULL, ":");
    char* time = strtok( trim(timeData), " " );
    int numTimes = 0;
    while( time != NULL ) {
        put( times, numTimes, atoll(time));
        time = strtok( NULL, " " );
        numTimes++;
    }
    // get the distance line
    getline(&line, &sizeOfLine, inputFile);

    // parsing the seed line to get just the data I need
    strtok(line, ":");
    char* distanceData = strtok(NULL, ":");
    char* distance = strtok( trim(distanceData), " " );
    int numDistances = 0;
    while( distance != NULL ) {
        put( distances, numDistances, atoll(distance));
        distance = strtok( NULL, " " );
        numDistances++;
    }

    int total = 1;
    for(int i = 0; i < numDistances; i++ ) {
        int measure = 0;
        int recordTime = get(times, i);
        int recordDistance = get(distances, i);
        for( int spd = 0; spd < recordTime; spd++ ) {
            int remainingTime = recordTime - spd;
            if( spd * remainingTime > recordDistance ) {
                measure++;
            }
        }
        total *= measure;
    }
    printf("total: %i", total);
}
