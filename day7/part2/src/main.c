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

typedef struct Hand {
    char* cards;
    int betSize;
    // hash that is used to give each hand a value;
    long long value;
    struct Hand* next;
} Hand;

long long cardValue(char card) {
    int returnValue = 0;
    switch (card) {
        case '2': returnValue = 1;
                  break;
        case '3': returnValue = 2;
                  break;
        case '4': returnValue = 3;
                  break;
        case '5': returnValue = 4;
                  break;
        case '6': returnValue = 5;
                  break;
        case '7': returnValue = 6;
                  break;
        case '8': returnValue = 7;
                  break;
        case '9': returnValue = 8;
                  break;
        case 'T': returnValue = 9;
                  break;
        case 'Q': returnValue = 10;
                  break;
        case 'K': returnValue = 11;
                  break;
        case 'A': returnValue = 12;
                  break;
    }
    return returnValue;
}

// calculate out the value of each hand
void handValue(Hand* hand) {
    HashMap* myMap = createHashMap(5);
    long long value = 0;
    char* possibleCard = "J23456789TQKA";
    int numJokers = 0;
    for(int i = 0; i < 5; i++ ) {
        char currCard = hand->cards[i];
        // if the current card is in the map, increment it's count
        if( currCard == 'J' ) {
            numJokers++;
        }
        else if( get(myMap, currCard) != -1 ) {
            put( myMap, currCard, get( myMap, currCard) + 1 );
        }
        // else set the count to 1;
        else {
            put(myMap, currCard, 1);
        }
        long long currCardValue = cardValue(currCard);
        for (int j = 1;j < 5 - i;j++) {
            currCardValue *=100;
        }
        value += currCardValue;
    }
    // int numJokers = get(myMap, 'J');
    // removePair(myMap, 'J');
    for( int i = 0; i < strlen(possibleCard); i++ ) {
        char currCard = possibleCard[i];
        int count = get( myMap, currCard );
        // depending on the count increase value of hand
        switch (count) {
            case 2: value += 10000000000;
                    break;
            case 3: value += 30000000000;
                    break;
            case 4: value += 50000000000;
                    break;
            case 5: value += 60000000000;
                    break;
            default: break;
        }
    }

    // 0 is high card, 1 is 1 pair, 2 is 2 pair, 3 is 3 of a kind, 4 is full house, 5 is 4 of a kind, 6 is 5 of a kind
    long long handType = value / 10000000000;
    switch (handType) {
        case 0: switch (numJokers) {
                    case 1: value += 10000000000;
                            break;
                    case 2: value += 30000000000;
                            break;
                    case 3: value += 50000000000;
                            break;
                    case 4: value += 60000000000;
                            break;
                    case 5: value += 60000000000;
                            break;
                }
                break;
        case 1: switch (numJokers) {
                    case 1: value += 20000000000;
                            break;
                    case 2: value += 40000000000;
                            break;
                    case 3: value += 50000000000;
                            break;
                }
                break;
        case 2: switch (numJokers) {
                    case 1: value += 20000000000;
                            break;
                }
                break;
        case 3: switch (numJokers) {
                    case 1: value += 20000000000;
                            break;
                    case 2: value += 30000000000;
                            break;
                }
                break;
        case 5: switch (numJokers) {
                    case 1: value += 10000000000;
                            break;
                }
                break;
    }
    hand->value = value;
}

Hand* parseHands(FILE *inputFile) {
    char *line = NULL;
    size_t sizeOfLine = 0;
    ssize_t statusOfRead;
    Hand *handHead = NULL;
    // read a line
    statusOfRead = getline(&line, &sizeOfLine, inputFile);
    while ( statusOfRead != -1 && strcmp(line, "\n") != 0) {
        char* numberPointer = NULL;
        // get the hand
        char* hand = strtok_r(trim(line), " ", &numberPointer);
        int bet = atoi(strtok_r(NULL, " ", &numberPointer));
        Hand* currHand = (Hand*) malloc(sizeof(Hand));
        currHand->betSize = bet;
        currHand->cards = hand;
        currHand->next = handHead;
        handValue(currHand);
        handHead = currHand;
        // read the next line
        statusOfRead = getline(&line, &sizeOfLine, inputFile);
    }
    return handHead;
}

void swapHands(Hand* hand1, Hand* hand2) {
    // Swap the fields of the two hands
    char* tempCards = hand1->cards;
    int tempBetSize = hand1->betSize;
    long long tempValue = hand1->value;

    hand1->cards = hand2->cards;
    hand1->betSize = hand2->betSize;
    hand1->value = hand2->value;

    hand2->cards = tempCards;
    hand2->betSize = tempBetSize;
    hand2->value = tempValue;
}

void selectionSort(Hand* head) {
    Hand* current = head;

    while (current != NULL) {
        Hand* minHand = current;
        Hand* temp = current->next;

        while (temp != NULL) {
            // Compare the values
            if (temp->value < minHand->value) {
                minHand = temp;
            }

            temp = temp->next;
        }

        // Swap the hands with the minimum value
        swapHands(current, minHand);

        current = current->next;
    }
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

    // parse the hands into a list
    Hand* hands = parseHands(inputFile);
    selectionSort(hands);

    long long winnings = 0;
    int rank = 1;
    while (hands != NULL) {
        winnings += (rank * hands->betSize);
        rank++;
        hands = hands->next;
    }
    printf("total winnings: %lli\n", winnings);
}
