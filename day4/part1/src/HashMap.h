#ifndef HASHMAP_H
#define HASHMAP_H

typedef struct Node {
    int key;
    int value;
    struct Node* next;
} Node;

typedef struct {
    int size;
    Node** array;
} HashMap;

HashMap* createHashMap(int size);
int hash(int key, int size);
void put(HashMap* map, int key, int value);
int get(HashMap* map, int key);
void removePair(HashMap* map, int key);
void removeAdj(HashMap* map, int key);
HashMap* copyHashMap(const HashMap* original);
void clearHashMap(HashMap* original);

#endif /* HASHMAP_H */

