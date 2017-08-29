#ifndef HASH_H_
#define HASH_H_

#define _XOPEN_SOURCE 500

#include <stdlib.h>
#include <stdio.h>
#include <limits.h>
#include <string.h>
#include <stdbool.h>

typedef struct entry_s entry_t;
struct entry_s
{
  	unsigned long key;
  	char *value;
    int count;
  	struct entry_s *currentItem;
};

typedef struct hashtable_s hashtable_t;
struct hashtable_s
{
  	int size;
  	struct entry_s **entry;
};

hashtable_t* createTable(int size);
entry_t* createItem(unsigned long key, char *value);

unsigned long hash(int hashtableSize, char* word);
bool setItem(hashtable_t* hashtable, char* value);
char* getValue(hashtable_t* hashtable, unsigned long key);
int getCount(hashtable_t* hashtable, unsigned long key);
void deleteTable(hashtable_t* hashtable);

#endif // HASH_H_
