// ============================================================
// Пример использования LRU-кеша
// ============================================================
// Программа демонстрирует работу LRU-кеша:
// 1. Создаётся кеш с заданной ёмкостью
// 2. Последовательно поступают запросы (ключи)
// 3. Каждый запрос вставляется в кеш через lru_put()
// 4. В конце выводится количество попаданий (cache hit)
//
// ============================================================

#include "lru_cache.h"
#include <stdio.h>
#include <stdlib.h>

int main()
{
    int capacity, n;
    scanf("%d %d", &capacity, &n);
    
    struct LRUCache_t* cache = lru_create(capacity);
    
    for (int i = 0; i < n; ++i)
    {
        int key;
        scanf("%d", &key);
        lru_put(cache, key, NULL);
    }
    
    printf("%d\n", lru_get_cache_hit(cache));
    
    lru_free(cache);
    return 0;
}
