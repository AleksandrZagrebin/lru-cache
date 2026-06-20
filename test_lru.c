// ============================================================
// Тесты для LRU-кеша
// ============================================================
// Набор тестов покрывает все ключевые сценарии использования.
//
// УПРАВЛЕНИЕ ПАМЯТЬЮ:
// Кеш НЕ освобождает value.
// Пользователь сам выделяет и освобождает память.
// В тестах мы вручную освобождаем все выделенные данные.
//
// Тесты:
// 1. Создание и уничтожение кеша
// 2. Вставка и получение элементов
// 3. Обновление существующих ключей
// 4. Механизм вытеснения при переполнении
// 5. Корректность порядка использования
// 6. Работа с отрицательными ключами
// 7. Хранение строковых значений
// 8. Кеш ёмкостью в один элемент
// 9. Проверка счётчика Cache_hit
// ============================================================

#include "lru_cache.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

static void test_create() {
    printf("TEST: lru_create ... ");
    struct LRUCache_t* cache = lru_create(3);
    assert(cache != NULL);
    assert(lru_size(cache) == 0);
    assert(lru_get_cache_hit(cache) == 0);
    lru_free(cache);
    printf("PASSED\n");
}

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
    assert(lru_get_cache_hit(cache) == 3);
    
    free(v1);
    free(v2);
    free(v3);
    lru_free(cache);
    printf("PASSED\n");
}

static void test_update() {
    printf("TEST: lru_put update ... ");
    struct LRUCache_t* cache = lru_create(3);
    
    int* v1 = malloc(sizeof(int)); *v1 = 100;
    int* v2 = malloc(sizeof(int)); *v2 = 200;
    
    lru_put(cache, 1, v1);
    assert(*(int*)lru_get(cache, 1) == 100);
    assert(lru_size(cache) == 1);
    assert(lru_get_cache_hit(cache) == 1);
    
    lru_put(cache, 1, v2);
    assert(*(int*)lru_get(cache, 1) == 200);
    assert(lru_size(cache) == 1);
    assert(lru_get_cache_hit(cache) == 2);
    
    free(v1);
    free(v2);
    lru_free(cache);
    printf("PASSED\n");
}

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
    assert(lru_get_cache_hit(cache) == 0);
    
    lru_put(cache, 4, v4);
    assert(lru_size(cache) == 3);
    assert(lru_get(cache, 1) == NULL);
    assert(*(int*)lru_get(cache, 2) == 200);
    assert(*(int*)lru_get(cache, 3) == 300);
    assert(*(int*)lru_get(cache, 4) == 400);
    assert(lru_get_cache_hit(cache) == 3);
    
    free(v1);
    free(v2);
    free(v3);
    free(v4);
    lru_free(cache);
    printf("PASSED\n");
}

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
    assert(lru_get_cache_hit(cache) == 0);
    
    lru_get(cache, 1);
    assert(lru_get_cache_hit(cache) == 1);
    
    lru_put(cache, 4, v4);
    
    assert(lru_size(cache) == 3);
    assert(*(int*)lru_get(cache, 1) == 100);
    assert(lru_get(cache, 2) == NULL);
    assert(*(int*)lru_get(cache, 3) == 300);
    assert(*(int*)lru_get(cache, 4) == 400);
    assert(lru_get_cache_hit(cache) == 3);
    
    free(v1);
    free(v2);
    free(v3);
    free(v4);
    lru_free(cache);
    printf("PASSED\n");
}

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
    assert(lru_get_cache_hit(cache) == 2);
    
    free(v1);
    free(v2);
    lru_free(cache);
    printf("PASSED\n");
}

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
    assert(lru_get_cache_hit(cache) == 3);
    
    lru_put(cache, 4, s4);
    assert(lru_get(cache, 1) == NULL);
    assert(strcmp((char*)lru_get(cache, 4), "New") == 0);
    assert(lru_get_cache_hit(cache) == 4);
    
    free(s1);
    free(s2);
    free(s3);
    free(s4);
    lru_free(cache);
    printf("PASSED\n");
}

static void test_capacity_one() {
    printf("TEST: capacity 1 ... ");
    struct LRUCache_t* cache = lru_create(1);
    
    int* v1 = malloc(sizeof(int)); *v1 = 100;
    int* v2 = malloc(sizeof(int)); *v2 = 200;
    
    lru_put(cache, 1, v1);
    assert(lru_size(cache) == 1);
    assert(*(int*)lru_get(cache, 1) == 100);
    assert(lru_get_cache_hit(cache) == 1);
    
    lru_put(cache, 2, v2);
    assert(lru_size(cache) == 1);
    assert(lru_get(cache, 1) == NULL);
    assert(*(int*)lru_get(cache, 2) == 200);
    assert(lru_get_cache_hit(cache) == 2);
    
    free(v1);
    free(v2);
    lru_free(cache);
    printf("PASSED\n");
}

static void test_cache_hit_counter() {
    printf("TEST: cache_hit counter ... ");
    struct LRUCache_t* cache = lru_create(3);
    
    int* v1 = malloc(sizeof(int)); *v1 = 100;
    int* v2 = malloc(sizeof(int)); *v2 = 200;
    int* v3 = malloc(sizeof(int)); *v3 = 300;
    int* v4 = malloc(sizeof(int)); *v4 = 400;
    int* v5 = malloc(sizeof(int)); *v5 = 500;
    
    lru_put(cache, 1, v1);
    lru_put(cache, 2, v2);
    lru_put(cache, 3, v3);
    assert(lru_get_cache_hit(cache) == 0);
    
    lru_get(cache, 1);
    assert(lru_get_cache_hit(cache) == 1);
    
    lru_get(cache, 2);
    assert(lru_get_cache_hit(cache) == 2);
    
    lru_get(cache, 99);
    assert(lru_get_cache_hit(cache) == 2);
    
    lru_get(cache, 3);
    assert(lru_get_cache_hit(cache) == 3);
    
    lru_put(cache, 4, v4);
    assert(lru_get_cache_hit(cache) == 3);
    
    lru_put(cache, 4, v5);
    assert(lru_get_cache_hit(cache) == 4);
    
    lru_get(cache, 4);
    assert(lru_get_cache_hit(cache) == 5);
    
    lru_get(cache, 2);
    assert(lru_get_cache_hit(cache) == 6);
    
    free(v1);
    free(v2);
    free(v3);
    free(v4);
    free(v5);
    lru_free(cache);
    printf("PASSED\n");
}

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
    test_cache_hit_counter();
    printf("\nALL LRU CACHE TESTS PASSED!\n");
    return 0;
}
