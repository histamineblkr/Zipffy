#include "hash.h"

/* Hashing function (dbj2) */
unsigned long hash(int hashtableSize, char* word)
{
    if (word == NULL) { return -1; }

    size_t len = strlen(word);
    if (len > 50) { return -1; }

    unsigned long hash = 5381;
    int c;

    while ((c = *word++))
        hash = ((hash << 5) + hash) + c; // hash * 33 + c

    return hash % hashtableSize;
}

/* Create a new hashtable */
hashtable_t* createTable(int size)
{
  	hashtable_t* hashtable = NULL;

  	if (size < 1) return NULL;

  	// Allocate the table itself
  	if ((hashtable = malloc(sizeof(hashtable_t))) == NULL)
    {
  		  return NULL;
  	}

  	// Allocate pointers to the head nodes
  	if ((hashtable->entry = malloc(sizeof(entry_t *) * size)) == NULL)
    {
  		  return NULL;
  	}

  	for (int i = 0; i < size; i++)
    {
  		  hashtable->entry[i] = NULL;
  	}

  	hashtable->size = size;

  	return hashtable;
}

/* Create a key-value entryItem */
entry_t* createItem(unsigned long key, char *value)
{
  	entry_t *thisItem;

  	if ((thisItem = malloc(sizeof(entry_t))) == NULL) { return NULL; }

  	if (!(thisItem->key = key)) { return NULL;	}

  	if ((thisItem->value = strdup((char*)value)) == NULL) { return NULL;	}

  	thisItem->currentItem = thisItem;
    thisItem->count = 1;

  	return thisItem;
}

/* Insert a key-value entryItem into a hash table */
bool setItem(hashtable_t* hashtable, char* value)
{
    unsigned long key = 0;
    entry_t *item = NULL;

    key = hash(hashtable->size, value);
    if (key == (unsigned long)(-1)) { return false; }

    item = hashtable->entry[key];

    // Item does not exist so create it
    if (item == NULL)
    {
        item = createItem(key, value);
        hashtable->entry[key] = item;
    }
    // Item exists and has the same value
    else if (item != NULL && (strcmp(item->value, value) == 0))
    {
        item->count += 1;
    }
    // Collision
    else if (item != NULL && (strcmp(item->value, value) != 0))
    {
        fprintf(stderr, "ERROR: A collision occured with the hash function.\n");
        return false;
    }

    return true;
}

/* Retrieve the value from a hash table. */
char* getValue(hashtable_t *hashtable, unsigned long key)
{
    entry_t *item;

  	// Get item
  	item = hashtable->entry[key];

  	// Does item exist?
  	if (item == NULL)
  		  return NULL;
    else
  		  return item->value;
}

int getCount(hashtable_t *hashtable, unsigned long key)
{
    entry_t *item;

    // Get item
    item = hashtable->entry[key];

    // Does item exist?
    if (item == NULL)
        return -1;
    else
        return item->count;
}

/* Delete hash table */
void deleteTable(hashtable_t* hashtable)
{
  	for (int i = 0; i < hashtable->size; i++)
    {
        if (hashtable->entry[i] != NULL)
            free(hashtable->entry[i]->value);
        free(hashtable->entry[i]);
    }

    free(hashtable->entry);
}
