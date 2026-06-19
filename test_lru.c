```c
// ============================================================
// Тесты для LRU-кеша
// ============================================================
// Набор тестов покрывает все ключевые сценарии использования:
// - Создание и уничтожение кеша
// - Вставка и получение элементов
// - Обновление существующих ключей
// - Механизм вытеснения (eviction) при переполнении
// - Корректность порядка использования (LRU order)
// - Работа с отрицательными ключами
// - Хранение строковых значений
// - Кеш ёмкостью в один элемент
//
// Каждый тест использует динамическую память и полностью
// освобождает её после завершения.
// ============================================================

#include "lru_cache.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

// Создание и уничтожение кеша
static void test_create() {
    printf("TEST: lru_create ... ");
    struct LRUCache_t* cache = lru_create(3);
    assert(cache != NULL);
    assert(lru_size(cache) == 0);
    lru_free(cache);
    printf("PASSED\n");
}

// Вставка и получение элементов
static void test_put_get() {
    printf("TEST: lru_put and get ... ");
    struct LRUCache_t* cache = lru_create(3);
    
    int* v1 = malloc(sizeof(int)); *v1 = 100;
    int* v2 = malloc(sizeof(int)); *v2 = 200;
    int* v3 = malloc(sizeof(int)); *v3 = 300;
    
    lru_put(cache, 1, v1);
    lru_put(cache, 2, v2);
    lru_put(cache, 3, v3);
    
    assert(lru_size(cache) == 3);
    assert(*(int*)lru_get(cache, 1) == 100);
    assert(*(int*)lru_get(cache, 2) == 200);
    assert(*(int*)lru_get(cache, 3) == 300);
    assert(lru_get(cache, 99) == NULL);
    
    lru_free(cache);
    printf("PASSED\n");
}

// Обновление существующих ключей
static void test_update() {
    printf("TEST: lru_put update ... ");
    struct LRUCache_t* cache = lru_create(3);
    
    int* v1 = malloc(sizeof(int)); *v1 = 100;
    int* v2 = malloc(sizeof(int)); *v2 = 200;
    
    lru_put(cache, 1, v1);
    assert(*(int*)lru_get(cache, 1) == 100);
    assert(lru_size(cache) == 1);
    
    lru_put(cache, 1, v2);
    assert(*(int*)lru_get(cache, 1) == 200);
    assert(lru_size(cache) == 1);
    
    lru_free(cache);
    printf("PASSED\n");
}

// Механизм вытеснения (eviction) при переполнении
static void test_eviction() {
    printf("TEST: lru_put eviction ... ");
    struct LRUCache_t* cache = lru_create(3);
    
    int* v1 = malloc(sizeof(int)); *v1 = 100;
    int* v2 = malloc(sizeof(int)); *v2 = 200;
    int* v3 = malloc(sizeof(int)); *v3 = 300;
    int* v4 = malloc(sizeof(int)); *v4 = 400;
    
    lru_put(cache, 1, v1);
    lru_put(cache, 2, v2);
    lru_put(cache, 3, v3);
    
    assert(lru_size(cache) == 3);
    
    lru_put(cache, 4, v4);
    assert(lru_size(cache) == 3);
    assert(lru_get(cache, 1) == NULL);      // 1 вытеснился
    assert(*(int*)lru_get(cache, 2) == 200);
    assert(*(int*)lru_get(cache, 3) == 300);
    assert(*(int*)lru_get(cache, 4) == 400);
    
    lru_free(cache);
    printf("PASSED\n");
}

// Корректность порядка использования (LRU order)
static void test_lru_order() {
    printf("TEST: LRU order ... ");
    struct LRUCache_t* cache = lru_create(3);
    
    int* v1 = malloc(sizeof(int)); *v1 = 100;
    int* v2 = malloc(sizeof(int)); *v2 = 200;
    int* v3 = malloc(sizeof(int)); *v3 = 300;
    int* v4 = malloc(sizeof(int)); *v4 = 400;
    
    lru_put(cache, 1, v1);
    lru_put(cache, 2, v2);
    lru_put(cache, 3, v3);
    
    // Обращаемся к ключу 1 — он становится самым свежим
    lru_get(cache, 1);
    
    lru_put(cache, 4, v4);
    
    assert(lru_size(cache) == 3);
    assert(*(int*)lru_get(cache, 1) == 100);
    assert(lru_get(cache, 2) == NULL);      // 2 вытеснился
    assert(*(int*)lru_get(cache, 3) == 300);
    assert(*(int*)lru_get(cache, 4) == 400);
    
    lru_free(cache);
    printf("PASSED\n");
}

// Работа с отрицательными ключами
static void test_negative_keys() {
    printf("TEST: negative keys ... ");
    struct LRUCache_t* cache = lru_create(3);
    
    int* v1 = malloc(sizeof(int)); *v1 = 100;
    int* v2 = malloc(sizeof(int)); *v2 = 200;
    
    lru_put(cache, -1, v1);
    lru_put(cache, -2, v2);
    
    assert(lru_size(cache) == 2);
    assert(*(int*)lru_get(cache, -1) == 100);
    assert(*(int*)lru_get(cache, -2) == 200);
    
    lru_free(cache);
    printf("PASSED\n");
}

// Хранение строковых значений
static void test_string_values() {
    printf("TEST: string values ... ");
    struct LRUCache_t* cache = lru_create(3);
    
    char* s1 = strdup("Hello");
    char* s2 = strdup("World");
    char* s3 = strdup("!!!");
    char* s4 = strdup("New");
    
    lru_put(cache, 1, s1);
    lru_put(cache, 2, s2);
    lru_put(cache, 3, s3);
    
    assert(lru_size(cache) == 3);
    assert(strcmp((char*)lru_get(cache, 1), "Hello") == 0);
    assert(strcmp((char*)lru_get(cache, 2), "World") == 0);
    assert(strcmp((char*)lru_get(cache, 3), "!!!") == 0);
    
    lru_put(cache, 4, s4);
    assert(lru_get(cache, 1) == NULL);
    assert(strcmp((char*)lru_get(cache, 4), "New") == 0);
    
    lru_free(cache);
    printf("PASSED\n");
}

// Кеш ёмкостью в один элемент
static void test_capacity_one() {
    printf("TEST: capacity 1 ... ");
    struct LRUCache_t* cache = lru_create(1);
    
    int* v1 = malloc(sizeof(int)); *v1 = 100;
    int* v2 = malloc(sizeof(int)); *v2 = 200;
    
    lru_put(cache, 1, v1);
    assert(lru_size(cache) == 1);
    assert(*(int*)lru_get(cache, 1) == 100);
    
    lru_put(cache, 2, v2);
    assert(lru_size(cache) == 1);
    assert(lru_get(cache, 1) == NULL);      // 1 вытеснился
    assert(*(int*)lru_get(cache, 2) == 200);
    
    lru_free(cache);
    printf("PASSED\n");
}

// Запуск всех тестов
int main() {
    printf("\n=== LRU CACHE TESTS ===\n\n");
    test_create();
    test_put_get();
    test_update();
    test_eviction();
    test_lru_order();
    test_negative_keys();
    test_string_values();
    test_capacity_one();
    printf("\nALL LRU CACHE TESTS PASSED!\n");
    return 0;
}
```
