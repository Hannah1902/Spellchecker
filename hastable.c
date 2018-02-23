#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "hashtable.h"

#define DEFAULT_SIZE 256


/*** Hash a string to an integer ***/
// Input
//    char *s: the string to hash
// Output
//    The integer hash code
size_t hash(const char *s) {
  
  size_t h = 0;
  size_t length = strlen(s);

  for (size_t i = 0; i < length; i++) {
    h = (32 * h) + tolower(s[i]);
  }
  
  return h;
}


/*** Create a new HashTable ***/
// Output
//    A pointer to the new table
HashTable *hashtableCreate() {

  HashTable *newTable = allocate(sizeof(HashTable));

  newTable->buckets = allocate(DEFAULT_SIZE * sizeof(HashTable));
  newTable->size = DEFAULT_SIZE;
  
  printf("hashtableCreate: %p\n", newTable);

  return newTable;
}


/*** Print a hashtable ***/
void hashtablePrint(HashTable *h) {
  int i;
  for (i = 0; i < h->size; i++) {
    HashNode *node = h->buckets[i];
    printf("Contents of bucket %d:\n", i);
    while (node != NULL) {
      printf("\t<%s>\n", node->key);
      node = node->next;
    }
  }
}


/*** Insert a key-value pair into a table ***/
// Inputs
//    HashTable *h: the hashtable performing the insertion
//    char *key: the key string
// Output
//    Nothing
void hashtableInsert(HashTable *h, char *key) {

  HashNode *node = hashLookup(h, key, 1);
  
  if (node != NULL) {
    node->key = allocate(strlen(key) + 1);
    strcpy(node->key, key);
  }
    
}

/*** Add other HashTable functions as necessary ***/

HashNode* hashLookup(HashTable *h, char *key, int create) {

  size_t bucketIndex = hash(key) % h->size - 1;
  
  HashNode *node = h->buckets[bucketIndex];

  if (create) {
    HashNode *node = allocate(sizeof(HashNode));
    node->key = allocate(strlen(key));

    strcpy(node->key, key);
    node->next = h->buckets[bucketIndex];
    h->buckets[bucketIndex] = node;
    
    return node;
  }
  
  while (node != NULL) {
    if (strcmp(node->key, key) == 0) {
      return node;
    }
    node = node->next;
  }
  
  return NULL;
}

/** Additional optional functionality to just find a match in the HashTable
 * Returns:
 *  1: if match is found
 *  0: if match is not found
 */
int hashMatch(HashTable *h, char *key) {
  size_t hashedKey = hash(key);

  size_t bucketIndex = hashedKey % h->size - 1;

  HashNode *node = h->buckets[bucketIndex];

  while (node != NULL) {
    if (strcmp(node->key, key) == 0) {
      return 1;
    }
    node = node->next;
  }
  
  return 0;
}

void *allocate(size_t size) {
  void* ptr = malloc(size);
  if (ptr == NULL) {
      printf("Memory Allocation Failure\n");
      exit(1);
    }
    return ptr;
}
