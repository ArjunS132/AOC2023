#include <stdio.h>
#include <stdlib.h>
#include <regex.h>
#include <string.h>
#include <stdbool.h>
#include "HashMap.h"

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

    // read first line
    statusOfRead = getline(&line, &sizeOfLine, inputFile);

    // sum
    unsigned long long sum = 0;

    // hashMaps for the lines
    HashMap* prevMap = createHashMap(50);
    HashMap* currMap = createHashMap(50);
    HashMap* nextMap = createHashMap(50);
    HashMap* symbolsMap = createHashMap(50);

    // setting up the regex
    regex_t numRegex;
    char numPattern[] = "[0-9]+";
    int numRes;
    // compile regex
    regcomp(&numRegex, numPattern, REG_EXTENDED);
    regmatch_t numberMatch;
    regmatch_t symbolMatch;
    // execute regex for numbers in initial line
    int offset = 0;
    while (regexec(&numRegex, line + offset, 1, &numberMatch, 0) == 0) {
        // Match found
        int start = numberMatch.rm_so + offset;
        int end = numberMatch.rm_eo + offset;

        char word[10] = "";
        strncpy(word, line + start, end - start);
        // put the word at all indices
        for( int j = start; j < end; j++) {
            put(currMap, j, atoi(word) );
        }

        offset = end;

        if (numberMatch.rm_eo - numberMatch.rm_so <= 1) {
            // Avoid infinite loop for zero-width matches
            offset++;
        }
    }

    char symbolPattern[] = "[^0-9.\n]";
    int symbolRes;
    regex_t symbolRegex;
    // compile regex
    regcomp(&symbolRegex, symbolPattern, REG_EXTENDED);
    // execute regex
    offset = 0;
    while (regexec(&symbolRegex, line + offset, 1, &symbolRegex, 0) == 0) {
        // Match found
        int start = symbolMatch.rm_so + offset;
        int end = symbolMatch.rm_eo + offset;

        // add the found symbol into a map. use this map to then compare
        put(symbolsMap, start, 1);

        offset = end;

        if (symbolMatch.rm_eo - symbolMatch.rm_so <= 1) {
            // Avoid infinite loop for zero-width matches
            offset++;
        }
    }
    int lineNum = 1;
    // while there are lines to read
    while (( statusOfRead = getline(&line, &sizeOfLine, inputFile) ) != -1) {

        // run the number regex on the new line that was found
        // numRes = regexec(&numRegex, line, 100, &numberMatch, 0);
        // set up the map for the next line


        // running to get the numbers for the next line
        offset = 0;
        while (regexec(&numRegex, line + offset, 1, &numberMatch, 0) == 0) {
            // Match found
            int start = numberMatch.rm_so + offset;
            int end = numberMatch.rm_eo + offset;

            char word[10] = "";
            strncpy(word, line + start, end - start);
            // put the word at all indices
            for( int j = start; j < end; j++) {
                put(nextMap, j, atoi(word) );
            }

            offset = end;

            if (numberMatch.rm_so == numberMatch.rm_eo) {
                // Avoid infinite loop for zero-width matches
                offset++;
            }
        }

        offset = 0;
        // running to get symbols for the next line
        for( int i = 0; i < strlen(line); i++) {
            // if there is a symbol at this index
            if( get(symbolsMap, i) != -1) {
                // index of the symbol
                int index = i;
                int end = symbolMatch.rm_eo + offset;
                // TODO: fix this. only loops once

                // if to the top left
                if( get(prevMap, index - 1) != -1 ) {
                    printf("in if 1 with num: %i\n", get(prevMap, index - 1) );
                    sum += get(prevMap, index - 1);
                    removeAdj(prevMap, index - 1);
                }
                // if right above
                if( get(prevMap, index) != -1 ) {
                    printf("in if 2 with num: %i\n", get(prevMap, index) );
                    sum += get(prevMap, index );
                    removeAdj(prevMap, index );
                }
                // if to the top right
                if( get(prevMap, index + 1) != -1 ) {
                    printf("in if 3 with num: %i\n", get(prevMap, index + 1) );
                    sum += get(prevMap, index + 1);
                    removeAdj(prevMap, index + 1);
                }
                // if to the left
                if( get(currMap, index - 1) != -1 ) {
                    printf("in if 4 with num: %i\n", get(currMap, index - 1) );
                    sum += get(currMap, index - 1);
                    removeAdj(currMap, index - 1);
                }
                // if to the right
                if( get(currMap, index + 1) != -1 ) {
                    printf("in if 5 with num: %i\n", get(currMap, index + 1) );
                    sum += get(currMap, index + 1);
                    removeAdj(currMap, index + 1);
                }
                // if to the bottom left
                if( get(nextMap, index - 1) != -1 ) {
                    printf("in if 6 with num: %i\n", get(nextMap, index - 1) );
                    sum += get(nextMap, index - 1);
                    removeAdj(nextMap, index - 1);
                }
                // if right below
                if( get(nextMap, index) != -1 ) {
                    printf("in if 7 with num: %i\n", get(nextMap, index) );
                    sum += get(nextMap, index );
                    removeAdj(nextMap, index );
                }
                // if to the bottom right
                if( get(nextMap, index + 1) != -1 ) {
                    printf("in if 8 with num: %i\n", get(nextMap, index + 1) );
                    sum += get(nextMap, index + 1);
                    removeAdj(nextMap, index + 1);
                }
                printf("%lld\n", sum);
                offset = end;
                if( index == end ) {
                    offset++;
                }
            }
        }

        // getting the next set of symbols
        clearHashMap(symbolsMap);
        offset = 0;
        while (regexec(&symbolRegex, line + offset, 1, &symbolMatch, 0) == 0) {
            // Match found
            int start = symbolMatch.rm_so + offset;
            int end = symbolMatch.rm_eo + offset;

            // add the found symbol into a map. use this map to then compare
            put(symbolsMap, start, 1);

            offset = end;

            if (symbolMatch.rm_eo - symbolMatch.rm_so <= 1) {
                // Avoid infinite loop for zero-width matches
                offset++;
            }
        }


        // copy the currMap into prevMap
        prevMap = copyHashMap( currMap );
        // copy the nextMap into currMap
        currMap = copyHashMap( nextMap );
        clearHashMap(nextMap);
        lineNum++;
    }
    printf("%lld\n", sum);
}
