#include "hash.h"

/* Hashing function (dbj2) */
unsigned long hash(long hashtableSize, char* word)
{
    if (word == NULL) { return -1; }

    size_t len = strlen(word);
    if (len > 50) { return -1; }

    unsigned long hash = 199;
    int c;

    while ((c = *word++))
        hash = ((hash << 5) + hash) + c; // hash * 33 + c

    return hash % hashtableSize;
}

/* Create a new hashtable */
hashtable_t* createTable(long size)
{
  	hashtable_t* hashtable = NULL;

  	if (size < 1) { return NULL; }

  	// Allocate the table itself
  	if ((hashtable = malloc(sizeof(hashtable_t))) == NULL) { return NULL; }

  	// Allocate pointers to the head nodes
  	if ((hashtable->entry = malloc(sizeof(entry_t*) * size)) == NULL)
  		  return NULL;

  	for (int i = 0; i < size; i++) { hashtable->entry[i] = NULL; }

  	hashtable->size = size;

  	return hashtable;
}

/* Create a key-value entryItem */
entry_t* createItem(unsigned long key, char *value)
{
    entry_t* newItem = NULL;

    if ((newItem = malloc(sizeof(entry_t))) == NULL) { return NULL; }

    if (!(newItem->key = key)) { return NULL;	}

    if ((newItem->value = strdup(value)) == NULL) { return NULL; }

    newItem->nextItem = NULL;
    newItem->count = 1;

    return newItem;
}

/* Insert a key-value entryItem into a hash table */
bool setItem(hashtable_t* hashtable, char* value)
{
    unsigned long key = 0;
    entry_t* newItem = NULL;
    entry_t* entryItem = NULL;

    key = hash(hashtable->size, value);
    if (key == (unsigned long)(-1)) { return false; }

    entryItem = hashtable->entry[key];

    // entryItem does not exist so create it and return true
    if (entryItem == NULL)
    {
        entryItem = createItem(key, value);
        hashtable->entry[key] = entryItem;
        entryItem->nextItem = NULL;
        return true;
    }
    // entryItem is the link list head and no other items exist
    else if (entryItem != NULL && entryItem->nextItem == NULL &&
             (strcmp(entryItem->value, value) != 0))
    {
        newItem = createItem(key, value);
        entryItem->nextItem = newItem;
        newItem->nextItem = NULL;
        return true;
    }
    // A link list exists with multiple items, traverse list and add newItem
    else
    {
        while (entryItem->nextItem != NULL && (strcmp(entryItem->value, value)) != 0)
        {
            entryItem = entryItem->nextItem;
        }

        // Found item, increase item count
        if (strcmp(entryItem->value, value) == 0)
        {
            entryItem->count++;
            return true;
        }
        // Item doesn't exist, add to end of list
        else
        {
            newItem = createItem(key, value);
            entryItem->nextItem = newItem;
            newItem->nextItem = NULL;
            return true;
        }
        // Some error
        fprintf(stderr, "ERROR: A collision occured with the hash function.\n");
        fprintf(stderr, "item->value: %s,\tvalue: %s\n", newItem->value, value);
        return false;
    }
    // Something went wrong
    fprintf(stderr, "ERROR: setItem(), an entry or linked list error occurred\n");
    return false;
}

/* Retrieve the count of an item from a hash table. */
int getCount(hashtable_t *hashtable, char* value)
{
    entry_t *entryItem;

    // Get item
    entryItem = hashtable->entry[hash(hashtable->size, value)];

    // Item does not exist
    if (entryItem == NULL)
    {
        return -1;
    }
    // Search linked list for correct item
    else
    {
        while (entryItem != NULL && (strcmp(entryItem->value, value) != 0))
        {
            entryItem = entryItem->nextItem;
        }

        // No item found
        if (entryItem == NULL)
          return -1;
        // Item found
        else
          return entryItem->count;
    }
}

/* Retrieve the key of an item from a hash table. */
unsigned long getKey(hashtable_t *hashtable, char* value)
{
    entry_t *entryItem;

    // Get item
    entryItem = hashtable->entry[hash(hashtable->size, value)];

    // Item does not exist
    if (entryItem == NULL)
    {
        return (unsigned long)-1;
    }
    // Search linked list for correct item
    else
    {
      while (entryItem != NULL && (strcmp(entryItem->value, value) != 0))
      {
          entryItem = entryItem->nextItem;
      }

      // No item found
      if (entryItem == NULL)
        return (unsigned long)-1;
      // Item found
      else
        return entryItem->key;
    }
}

/* Delete hash table */
void deleteTable(hashtable_t* hashtable)
{
  	for (int i = 0; i < hashtable->size; i++)
    {
        entry_t* currentItem = hashtable->entry[i];
        if (currentItem != NULL)
        {
            if (currentItem->nextItem == NULL)
            {
                free(currentItem->value);
                currentItem->value = NULL;
            }
            else
            {
                entry_t* nextItem = NULL;
                while (currentItem != NULL)
                {
                    nextItem = currentItem->nextItem;
                    free(currentItem->value);
                    free(currentItem);
                    currentItem = nextItem;
                    nextItem = NULL;
                }
            }
        }
        free(currentItem);
    }
    free(hashtable->entry);
}
