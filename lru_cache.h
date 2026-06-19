#ifndef LRU_CACHE_H
#define LRU_CACHE_H

struct LRUCache_t;

struct LRUCache_t* lru_create(int capacity);
void lru_free(struct LRUCache_t* cache);

void* lru_get(struct LRUCache_t* cache, int key);
void lru_put(struct LRUCache_t* cache, int key, void* value);
int lru_size(struct LRUCache_t* cache);
int lru_get_cache_hit(struct LRUCache_t* cache);
#endif
