#include <stdio.h>
#include <stdlib.h>
#include <regex.h>
#include <string.h>

// find index of an ele in an array
int findIndex(char** words, char* searchTerm) {
    int result = -1;
    int index = -1;
    for (int i = 0; i < 10 && result != 0; i++) {
        result = strcmp(words[i], searchTerm);
        index = i;
    }
    return index;
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
    int sum = 0;
    char *convert[10] = { "", "one", "two", "three", "four", "five", "six", "seven", "eight", "nine" };

    // while there are files to read
    while (( statusOfRead = getline(&line, &sizeOfLine, inputFile) ) != -1) {
        // set up the regex
        int offset = 0;
        regex_t regex;
        char pattern[] = "[0-9]|one|two|three|four|five|six|seven|eight|nine";
        // char pattern[] = "(?:[0-9]|one|two|three|four|five|six|seven|eight|nine)";
        // char pattern[] = "[0-9]";
        // char pattern[] = "one";
        int result;
        // compile regex
        result = regcomp(&regex, pattern, REG_EXTENDED);
        regmatch_t matches[100];
        // execute regex
        result = regexec(&regex, line, 100, matches, 0);
        int indexOfFirstDigit = matches[0].rm_so;
        int firstDigitValue = 0;
        int lastDigitValue = 0;

        // if the length of what is found is > 1 then it's more then a single char
        if( matches[0].rm_eo - matches[0].rm_so > 1) {
            // the word for the number
            char word[10] = "";
            strncpy(word, line + matches[0].rm_so, matches[0].rm_eo - matches[0].rm_so);
            firstDigitValue = findIndex( convert, word );
        }
        else {
            char firstDigitChar = line[indexOfFirstDigit];
            firstDigitValue = firstDigitChar - '0';
        }

        int lastDigitIndex = indexOfFirstDigit;
        offset = indexOfFirstDigit;
        result = regexec(&regex, line + offset, 100, matches, 0);
        int length;
        while(result == 0) {
            lastDigitIndex = matches[0].rm_so + offset;
            length = matches[0].rm_eo - matches[0].rm_so;
            offset = lastDigitIndex + 1;
            result = regexec(&regex, line + offset, 100, matches, 0);
        }
        // if the length of what is found is > 1 then it's more then a single char
        if(length > 1) {
            // the word for the number
            char word[10] = "";
            strncpy(word, line + lastDigitIndex, length);
            lastDigitValue = findIndex( convert, word );
        }
        else {
            char lastDigitChar = line[lastDigitIndex];
            lastDigitValue = lastDigitChar - '0';
        }
        printf("Line: %s", line);
        printf("firstDigitValue: %i", firstDigitValue);
        printf("\n");
        printf("lastDigitChar: %i", lastDigitValue);
        printf("\n");
        sum += firstDigitValue*10 + lastDigitValue;
        regfree(&regex);
    }
    printf("sum: %i", sum);
    fclose(inputFile);
    free(line);
    return 0;
}
