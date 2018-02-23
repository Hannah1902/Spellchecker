// Header file with hashtable definitions

typedef struct hashnode_t {
  char *key;
  struct hashnode_t *next;
} HashNode;

typedef struct hashtable_t {
  HashNode **buckets;
  int size;
} HashTable;

HashTable *hashtableCreate();
void hashtableInsert(HashTable*, char*);
void hashtablePrint(HashTable*);
unsigned long int hash(const char*);
HashNode* hashLookup(HashTable*, char*, int);
int hashMatch(HashTable*, char*);
void* allocate(size_t);
