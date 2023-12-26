#ifndef HASHMAP_H
#define HASHMAP_H

typedef struct Node {
    long long key;
    long long value;
    struct Node* next;
} Node;

typedef struct {
    long long size;
    Node** array;
} HashMap;

HashMap* createHashMap(long long size);
long long hash(long long key, long long size);
void put(HashMap* map, long long key, long long value);
long long get(HashMap* map, long long key);
void removePair(HashMap* map, long long key);
void removeAdj(HashMap* map, long long key);
HashMap* copyHashMap(const HashMap* original);
void clearHashMap(HashMap* original);

#endif /* HASHMAP_H */

