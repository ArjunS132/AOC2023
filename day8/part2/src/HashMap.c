#include <stdio.h>
#include <stdlib.h>

typedef struct Node {
    long long key;
    long long value;
    struct Node* next;
} Node;

typedef struct {
    long long size;
    Node** array;
} HashMap;

HashMap* createHashMap(long long size) {
    HashMap* map = (HashMap*)malloc(sizeof(HashMap));
    map->size = size;
    map->array = (Node**)malloc(size * sizeof(Node*));
    for (long long i = 0; i < size; i++) {
        map->array[i] = NULL;
    }
    return map;
}

long long hash(long long key, long long size) {
    return key % size;
}

void put(HashMap* map, long long key, long long value) {
    long long index = hash(key, map->size);
    Node* node = (Node*)malloc(sizeof(Node));
    node->key = key;
    node->value = value;
    node->next = map->array[index];
    map->array[index] = node;
}

long long get(HashMap* map, long long key) {
    long long index = hash(key, map->size);
    Node* tempNode = map->array[index];
    while (key != -1 && tempNode != NULL) {
        if (tempNode->key == key) {
            return tempNode->value;
        }
        tempNode = tempNode->next;
    }
    return -1; // Key not found
}

void removePair(HashMap* map, long long key) {
    long long index = hash(key, map->size);
    Node* current = map->array[index];
    Node* prev = NULL;

    while (current != NULL && current->key != key) {
        prev = current;
        current = current->next;
    }

    if (current == NULL) {
        // Key not found
        return;
    }

    if (prev == NULL) {
        // The node to be removed is the first in the list
        map->array[index] = current->next;
    } else {
        prev->next = current->next;
    }

    free(current);
}

void removeAdj(HashMap* map, long long key) {
    long long index = key;
    // remove to the left
    while( get(map, index) != -1 ) {
        removePair(map, index);
        index--;
    }
    // remove to the right
    index = key + 1;
    while( get(map, index) != -1 ) {
        removePair(map, index);
        index++;
    }
}

HashMap* copyHashMap(const HashMap* original) {
    if (original == NULL) {
        return NULL;
    }

    HashMap* copy = createHashMap(original->size);

    for (long long i = 0; i < original->size; i++) {
        Node* current = original->array[i];
        while (current != NULL) {
            put(copy, current->key, current->value);
            current = current->next;
        }
    }

    return copy;
}

void clearHashMap(HashMap* map) {
    long long size = map->size;
    for (long long i = 0; i < size; i++) {
        map->array[i] = NULL;
    }
}
