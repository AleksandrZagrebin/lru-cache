#ifndef HASHTABLE_H
#define HASHTABLE_H

struct Hashtable_t;

struct HashTable_t* hash_table_create(int size);
void hash_table_free(struct HashTable_t* ht);

// ============================================
// Основные операции
// ============================================

void hash_table_put(struct HashTable_t* ht, int key, void* value);
void* hash_table_get(struct HashTable_t* ht, int key);
int hash_table_remove(struct HashTable_t* ht, int key);

// ============================================
// Вспомогательные функции
// ============================================
int hash_table_size(struct HashTable_t* ht);
void hash_table_clear(struct HashTable_t* ht);
int hash_table_count(struct HashTable_t* ht);
void hash_table_print(struct HashTable_t* ht);

#endif
