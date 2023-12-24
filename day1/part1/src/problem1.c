
#include <stdio.h>
#include <stdlib.h>
#include <regex.h>

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

    // while there are files to read
    while (( statusOfRead = getline(&line, &sizeOfLine, inputFile) ) != -1) {
        // set up the regex
        int offset = 0;
        regex_t regex;
        char pattern[] = "[0-9]";
        int result;
        result = regcomp(&regex, pattern, 0);
        regmatch_t matches[100];
        result = regexec(&regex, line, 100, matches, 0);
        int indexOfFirstDigit = matches[0].rm_so;

        int lastDigitIndex = indexOfFirstDigit;
        offset = indexOfFirstDigit;
        result = regexec(&regex, line + offset, 100, matches, 0);
        while(result == 0) {
            lastDigitIndex = matches[0].rm_so + offset;
            offset = lastDigitIndex + 1;
            result = regexec(&regex, line + offset, 100, matches, 0);
        }
        char firstDigitChar = line[indexOfFirstDigit];
        int firstDigitValue = firstDigitChar - '0';
        char lastDigitChar = line[lastDigitIndex];
        int lastDigitValue = lastDigitChar - '0';
        printf("Line: %s", line);
        printf("firstDigitChar: %i", firstDigitValue);
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
