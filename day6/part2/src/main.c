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
    // input file
    FILE *inputFile = fopen("src/input.txt", "rb");

    // making sure file opens
    if (inputFile == NULL) {
        perror("Error opening file");
        return 1;
    }
    size_t sizeOfLine = 0;


    char* line = NULL;
    // get the time line
    getline(&line, &sizeOfLine, inputFile);

    // parsing the seed line to get just the data I need
    strtok(line, ":");
    char* timeData = strtok(NULL, ":");
    char* time = strtok( trim(timeData), " " );
    char* totalTime = (char*) malloc(0);
    while( time != NULL ) {
        totalTime = realloc(totalTime, strlen(totalTime) + strlen(time) );
        strcat( totalTime, time);
        time = strtok( NULL, " " );
    }
    long long recordTime = atoll(totalTime);
    // get the distance line
    getline(&line, &sizeOfLine, inputFile);

    // parsing the seed line to get just the data I need
    strtok(line, ":");
    char* distanceData = strtok(NULL, ":");
    char* distance = strtok( trim(distanceData), " " );
    char* totalDistance = (char*) malloc(0);
    while( distance != NULL ) {
        totalDistance = realloc(totalDistance, strlen(totalDistance) + strlen(distance) );
        strcat(totalDistance, distance);
        distance = strtok( NULL, " " );
    }

    long long recordDistance = atoll(totalDistance);
    long long measure = 0;
    for( long long spd = 0; spd < recordTime; spd++ ) {
        int remainingTime = recordTime - spd;
        if( spd * remainingTime > recordDistance ) {
            measure++;
        }
    }
    printf("total: %lli", measure);

}
