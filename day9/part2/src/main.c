#include <stdio.h>
#include <stdlib.h>
#include <regex.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>

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

typedef struct Node {
    long long value;
    struct Node* next;
}Node;

void printList(Node* list) {
    Node* copy = list;
    while( copy != NULL ) {
        printf("%lli ", copy->value);
        copy = copy->next;
    }
    printf("\n");
}

Node* reverseLinkedList(Node* list) {
    Node* headNode = NULL;
    while( list != NULL ) {
        Node* currNode = (Node*) malloc(sizeof(Node*));
        currNode->next = headNode;
        currNode->value = list->value;
        headNode = currNode;
        list = list->next;
    }
    free(list);
    return headNode;

}

// read a line and make a list
Node* parseLine(char *line) {
    char* linePtr = NULL;
    Node* headNode = NULL;
    char* numAsChar = strtok_r(line, " ", &linePtr);
    while( numAsChar != NULL ) {
        long long numAsLong = atoll(numAsChar);
        Node* currNode = (Node*) malloc(sizeof(Node*));
        currNode->value = numAsLong;
        currNode->next = headNode;
        headNode = currNode;
        numAsChar = strtok_r(NULL, " ", &linePtr);
    }
    return (headNode);
}

// returns true if list is all 0's. else return false;
bool allZero(Node* list) {
    Node* copyList = list;
    bool allZeros = true;
    while( allZeros && copyList != NULL ) {
        allZeros = copyList->value == 0;
        copyList = copyList->next;
    }
    return allZeros;
}

// calculate the next number in any given list. (recursive);
long long calcNextNum(Node* list) {
    Node* copy = list;
    if(allZero(copy)) {
        return 0;
    }
    else {
        Node* headNode = NULL;
        while( copy->next != NULL ) {
            Node* currNode = (Node*) malloc(sizeof(Node*));
            // the calculation
            long long value = copy->next->value - copy->value;
            currNode->next = headNode;
            currNode->value = value;
            headNode = currNode;
            copy = copy->next;
        }
        // headNode = (headNode);
        // at this point the headNode is the last value in the list
        return copy->value + calcNextNum(reverseLinkedList(headNode));
    }
    free(copy);
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
    // read a line
    statusOfRead = getline(&line, &sizeOfLine, inputFile);
    long long sum = 0;
    while (statusOfRead != -1) {
        Node* currList = parseLine(line);
        sum += calcNextNum(currList);
        free(currList);
        statusOfRead = getline(&line, &sizeOfLine, inputFile);
    }
    printf("sum: %lli\n", sum);
}

