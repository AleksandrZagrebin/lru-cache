// ============================================================
// LRU-кеш (Least Recently Used)
// ============================================================
// Объединяет хеш-таблицу и двусвязный список для эффективного
// управления ограниченным объёмом памяти.
//
// Компоненты:
// - Хеш-таблица: быстрый поиск элементов по ключу за O(1)
// - Двусвязный список: поддержание порядка использования
// - Счётчик Cache_hit: количество попаданий в кеш
//
// Принцип работы:
// - Голова списка — самый свежий элемент
// - Хвост списка — самый старый элемент
// - При обращении элемент перемещается в голову
// - При переполнении удаляется элемент из хвоста
//
// УПРАВЛЕНИЕ ПАМЯТЬЮ:
// Кеш НЕ выделяет и НЕ освобождает value.
// Пользователь сам выделяет и освобождает данные.
// Кеш освобождает только свои внутренние структуры.
// ============================================================

#include <stdio.h>
#include <stdlib.h>
#include "list.h"
#include "hashtable.h"

// Структура LRU-кеша (полностью инкапсулирована)
struct LRUCache_t {
    struct HashTable_t* ht;   // хеш-таблица для поиска
    struct List_t* list;      // двусвязный список для порядка
    int Cache_hit;            // счётчик попаданий
    int capacity;             // максимальная ёмкость
    int size;                 // текущий размер
};

// Создание кеша с заданной ёмкостью
struct LRUCache_t* lru_create(int capacity) {
    if (capacity <= 0) return NULL;
    
    struct LRUCache_t* cache = (struct LRUCache_t*)malloc(sizeof(struct LRUCache_t));
    if (!cache) return NULL;
    
    cache->ht = hash_table_create(capacity * 2);
    if (!cache->ht) {
        free(cache);
        return NULL;
    }
    
    cache->list = list_create();
    if (!cache->list) {
        hash_table_free(cache->ht);
        free(cache);
        return NULL;
    }
    
    cache->capacity = capacity;
    cache->size = 0;
    cache->Cache_hit = 0;
    
    return cache;
}

// Полное освобождение кеша
void lru_free(struct LRUCache_t* cache) {
    if (!cache) return;
    list_free(cache->list);      // список освобождает только узлы, НЕ value
    hash_table_free(cache->ht);  // хеш-таблица освобождает только свои узлы
    free(cache);
}

// Получение элемента по ключу
void* lru_get(struct LRUCache_t* cache, int key) {
    if (!cache) return NULL;
    
    struct ListNode_t* node = (struct ListNode_t*)hash_table_get(cache->ht, key);
    if (!node) return NULL;
    
    cache->Cache_hit++;
    list_move_to_front(cache->list, node);
    return node->value;
}

// Вставка или обновление элемента по ключу
void lru_put(struct LRUCache_t* cache, int key, void* value) {
    if (!cache) return;
    
    struct ListNode_t* node = (struct ListNode_t*)hash_table_get(cache->ht, key);
    
    if (node) {
        // Cache hit — обновляем указатель на данные
        node->value = value;
        list_move_to_front(cache->list, node);
        cache->Cache_hit++;
        return;
    }
    
    // Cache miss — создаём новый узел
    node = list_push_front(cache->list, key, value);
    if (!node) {
        return;  // value остаётся у пользователя, он сам решит что делать
    }
    
    hash_table_put(cache->ht, key, (void*)node);
    cache->size++;
    
    // Если кеш переполнен — удаляем самый старый элемент (хвост)
    if (cache->size > cache->capacity) {
        struct ListNode_t* tail = list_tail(cache->list);
        if (tail) {
            int old_key = tail->key;
            hash_table_remove(cache->ht, old_key);
            list_pop_back(cache->list);  // удаляем узел, value не трогаем
            cache->size--;
        }
    }
}

// Получение текущего количества элементов
int lru_size(struct LRUCache_t* cache) {
    return cache ? cache->size : 0;
}

// Получение количества попаданий в кеш
int lru_get_cache_hit(struct LRUCache_t* cache) {
    return cache ? cache->Cache_hit : 0;
}
