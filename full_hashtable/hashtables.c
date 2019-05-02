#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
  Hash table key/value pair with linked list pointer.
  Note that an instance of `LinkedPair` is also a node in a linked list.
  More specifically, the `next` field is a pointer pointing to the the
  next `LinkedPair` in the list of `LinkedPair` nodes.
 */
typedef struct LinkedPair
{
  char *key;
  char *value;
  struct LinkedPair *next;
} LinkedPair;

/*
  Hash table with linked pairs.
 */
typedef struct HashTable
{
  int capacity;
  int count;
  LinkedPair **storage;
} HashTable;

/*
  Create a key/value linked pair to be stored in the hash table.
 */
LinkedPair *create_pair(char *key, char *value)
{
  LinkedPair *pair = (LinkedPair *)malloc(sizeof(LinkedPair));
  pair->key = strdup(key);
  pair->value = strdup(value);
  pair->next = NULL;

  return pair;
}

/*
  Use this function to safely destroy a hashtable pair.
 */
void destroy_pair(LinkedPair *pair)
{
  if (pair != NULL)
  {
    free(pair->key);
    free(pair->value);
    free(pair);
    pair = NULL;
  }
}

/*
  djb2 hash function
  Do not modify this!
 */
unsigned int hash(char *str, int max)
{
  unsigned long hash = 5381;
  int c;
  unsigned char *u_str = (unsigned char *)str;

  while ((c = *u_str++))
  {
    hash = ((hash << 5) + hash) + c;
  }

  return hash % max;
}

/*
  Fill this in.
  All values in storage should be initialized to NULL
 */
HashTable *create_hash_table(int capacity)
{
  if (capacity < 1)
  {
    return NULL;
  }

  HashTable *ht = (HashTable *)malloc(sizeof(HashTable));
  ht->capacity = capacity;
  ht->count = 0;
  ht->storage = (LinkedPair **)calloc(capacity, sizeof(LinkedPair *));

  return ht;
}

/*
  Fill this in.
  Inserting values to the same index with different keys should be
  added to the corresponding LinkedPair list.
  Inserting values to the same index with existing keys can overwrite
  the value in th existing LinkedPair list.
 */
void hash_table_insert(HashTable *ht, char *key, char *value)
{

  unsigned int index = hash(key, ht->capacity);

  if (ht->storage[index] == NULL)
  {
    LinkedPair *new_pair = create_pair(key, value);
    ht->storage[index] = new_pair;
    ht->count++;
    return;
  }

  
  LinkedPair *curr_pair = ht->storage[index];
  LinkedPair *next_pair = curr_pair->next;

  while (next_pair != NULL && strcmp(curr_pair->key, key) != 0)
  {
    curr_pair = next_pair;
    next_pair = curr_pair->next;
  }

  
  if (strcmp(curr_pair->key, key) == 0)
  {
    printf("key is already in use, value will be overwritten...\n");
    curr_pair->value = value;
  }
  else
  {
    LinkedPair *new_pair = create_pair(key, value);
    curr_pair->next = new_pair;
  }
}

/*
  Fill this in.
  Should search the entire list of LinkedPairs for existing
  keys and remove matching LinkedPairs safely.
  Don't forget to free any malloc'ed memory!
 */
void hash_table_remove(HashTable *ht, char *key)
{
  unsigned int index = hash(key, ht->capacity);

  if (ht->storage[index] == NULL)
  {
    printf("Key \"%s\" was not found in the hash table\n", key);
    return;
  }

  LinkedPair *curr_node = ht->storage[index];
  if (strcmp(curr_node->key, ht->storage[index]->key) == 0)
  {
    ht->storage[index] = curr_node->next;
    destroy_pair(curr_node);
    return;
  }

  LinkedPair *prev_node = curr_node;
  curr_node = prev_node->next;

  while (curr_node != NULL && strcmp(curr_node->key, key) != 0)
  {
    prev_node = curr_node;
    curr_node = prev_node->next;
  }

  if (strcmp(curr_node->key, key) == 0)
  {
    prev_node->next = curr_node->next;
    destroy_pair(curr_node);
    curr_node = NULL;
  }
  else
  {
    printf("Key \"%s\" was not found in the Link List\n", key);
  }
}

/*
  Fill this in.
  Should search the entire list of LinkedPairs for existing
  keys.
  Return NULL if the key is not found.
 */
char *hash_table_retrieve(HashTable *ht, char *key)
{
  unsigned int index = hash(key, ht->capacity);

  if (ht->storage[index] == NULL)
  {
    printf("Key \"%s\" was not found in the hash table\n", key);
    return NULL;
  }

  LinkedPair *node = ht->storage[index];
  while (node != NULL && strcmp(node->key, key) != 0)
  {
    node = node->next;
  }

  if (node != NULL)
  {
    return node->value;
  }

  return NULL;
}

/*
  Fill this in.
  Don't forget to free any malloc'ed memory!
 */
void destroy_hash_table(HashTable *ht)
{
  for (int i = 0; i < ht->capacity; i++)
  {
    if (ht->storage[i] != NULL)
    {
      LinkedPair *curr_node = ht->storage[i];
      LinkedPair *next_node = curr_node->next;
      ht->storage[i] = NULL;
      while (next_node != NULL)
      {
        destroy_pair(curr_node);
        curr_node = next_node;
        next_node = curr_node->next;
      }
      destroy_pair(curr_node);
    }
  }

  // free the storage
  free(ht->storage);
  ht->storage = NULL;

  // free the hash table
  free(ht);
  ht = NULL;
}

/*
  Fill this in.
  Should create a new hash table with double the capacity
  of the original and copy all elements into the new hash table.
  Don't forget to free any malloc'ed memory!
 */
HashTable *hash_table_resize(HashTable *ht)
{
  HashTable *new_ht = create_hash_table(ht->capacity * 2);

  for (int i = 0; i < ht->capacity; i++)
  {
    if (ht->storage[i] != NULL)
    {
      LinkedPair *node = ht->storage[i];
      while (node != NULL)
      {
        hash_table_insert(new_ht, node->key, node->value);
        node = node->next;
      }
    }
  }

  // Destroy old hash table
  destroy_hash_table(ht);

  return new_ht;
}

#ifndef TESTING
int main(void)
{
  struct HashTable *ht = create_hash_table(2);

  hash_table_insert(ht, "line_1", "Tiny hash table\n");
  hash_table_insert(ht, "line_2", "Filled beyond capacity\n");
  hash_table_insert(ht, "line_3", "Linked list saves the day!\n");

  printf("%s", hash_table_retrieve(ht, "line_1"));
  printf("%s", hash_table_retrieve(ht, "line_2"));
  printf("%s", hash_table_retrieve(ht, "line_3"));

  int old_capacity = ht->capacity;
  ht = hash_table_resize(ht);
  int new_capacity = ht->capacity;

  printf("\nResizing hash table from %d to %d.\n", old_capacity, new_capacity);

  destroy_hash_table(ht);

  return 0;
}
#endif