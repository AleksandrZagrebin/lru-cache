// ============================================================
// Тесты для хеш-таблицы (освобождающая версия)
// ============================================================
// Набор тестов покрывает все основные сценарии использования
// хеш-таблицы. Каждый тест использует динамическую память
// и полностью освобождает её после завершения.
//
// Тесты:
// 1. Создание и уничтожение таблицы
// 2. Вставка и поиск значений
// 3. Обновление существующих ключей
// 4. Удаление элементов
// 5. Коллизии (несколько ключей в одной корзине)
// 6. Удаление из середины цепочки
// 7. Очистка всех элементов
// 8. Отрицательные ключи
// 9. Автоматическое рехеширование
// 10. Создание с нулевым размером
// 11. Операции с NULL-таблицей
// ============================================================

#include "hashtable.h"
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

// Создание и уничтожение таблицы
static void test_create() {
    printf("TEST: hash_table_create ... ");
    struct HashTable_t* ht = hash_table_create(10);
    assert(ht != NULL);
    assert(hash_table_size(ht) > 0);
    assert(hash_table_count(ht) == 0);
    hash_table_free(ht);
    printf("PASSED\n");
}

// Вставка и поиск значений
static void test_put_get() {
    printf("TEST: hash_table_put and get ... ");
    struct HashTable_t* ht = hash_table_create(10);
    int* v1 = malloc(sizeof(int)); *v1 = 100;
    int* v2 = malloc(sizeof(int)); *v2 = 200;
    int* v3 = malloc(sizeof(int)); *v3 = 300;
    hash_table_put(ht, 1, v1);
    hash_table_put(ht, 2, v2);
    hash_table_put(ht, 3, v3);
    assert(hash_table_count(ht) == 3);
    assert(*(int*)hash_table_get(ht, 1) == 100);
    assert(*(int*)hash_table_get(ht, 2) == 200);
    assert(*(int*)hash_table_get(ht, 3) == 300);
    assert(hash_table_get(ht, 99) == NULL);
    hash_table_free(ht);
    printf("PASSED\n");
}

// Обновление существующих ключей
static void test_update() {
    printf("TEST: hash_table_put update ... ");
    struct HashTable_t* ht = hash_table_create(10);
    int* v1 = malloc(sizeof(int)); *v1 = 100;
    int* v2 = malloc(sizeof(int)); *v2 = 200;
    hash_table_put(ht, 1, v1);
    assert(*(int*)hash_table_get(ht, 1) == 100);
    assert(hash_table_count(ht) == 1);
    hash_table_put(ht, 1, v2);
    assert(*(int*)hash_table_get(ht, 1) == 200);
    assert(hash_table_count(ht) == 1);
    hash_table_free(ht);
    printf("PASSED\n");
}

// Удаление элементов
static void test_remove() {
    printf("TEST: hash_table_remove ... ");
    struct HashTable_t* ht = hash_table_create(10);
    int* v1 = malloc(sizeof(int)); *v1 = 100;
    int* v2 = malloc(sizeof(int)); *v2 = 200;
    int* v3 = malloc(sizeof(int)); *v3 = 300;
    hash_table_put(ht, 1, v1);
    hash_table_put(ht, 2, v2);
    hash_table_put(ht, 3, v3);
    assert(hash_table_count(ht) == 3);
    int removed = hash_table_remove(ht, 2);
    assert(removed == 1);
    assert(hash_table_count(ht) == 2);
    assert(hash_table_get(ht, 2) == NULL);
    assert(*(int*)hash_table_get(ht, 1) == 100);
    assert(*(int*)hash_table_get(ht, 3) == 300);
    removed = hash_table_remove(ht, 99);
    assert(removed == 0);
    assert(hash_table_count(ht) == 2);
    hash_table_free(ht);
    printf("PASSED\n");
}

// Коллизии (несколько ключей в одной корзине)
static void test_collisions() {
    printf("TEST: hash_table collisions ... ");
    struct HashTable_t* ht = hash_table_create(10);
    int* v1 = malloc(sizeof(int)); *v1 = 100;
    int* v2 = malloc(sizeof(int)); *v2 = 200;
    int* v3 = malloc(sizeof(int)); *v3 = 300;
    hash_table_put(ht, 2, v1);
    hash_table_put(ht, 13, v2);
    hash_table_put(ht, 24, v3);
    assert(hash_table_count(ht) == 3);
    assert(*(int*)hash_table_get(ht, 2) == 100);
    assert(*(int*)hash_table_get(ht, 13) == 200);
    assert(*(int*)hash_table_get(ht, 24) == 300);
    hash_table_free(ht);
    printf("PASSED\n");
}

// Удаление из середины цепочки при коллизии
static void test_remove_from_chain() {
    printf("TEST: hash_table_remove from collision chain ... ");
    struct HashTable_t* ht = hash_table_create(10);
    int* v1 = malloc(sizeof(int)); *v1 = 100;
    int* v2 = malloc(sizeof(int)); *v2 = 200;
    int* v3 = malloc(sizeof(int)); *v3 = 300;
    int* v4 = malloc(sizeof(int)); *v4 = 400;
    hash_table_put(ht, 2, v1);
    hash_table_put(ht, 13, v2);
    hash_table_put(ht, 24, v3);
    hash_table_put(ht, 35, v4);
    assert(hash_table_count(ht) == 4);
    int removed = hash_table_remove(ht, 24);
    assert(removed == 1);
    assert(hash_table_count(ht) == 3);
    assert(hash_table_get(ht, 24) == NULL);
    assert(*(int*)hash_table_get(ht, 2) == 100);
    assert(*(int*)hash_table_get(ht, 13) == 200);
    assert(*(int*)hash_table_get(ht, 35) == 400);
    hash_table_free(ht);
    printf("PASSED\n");
}

// Очистка всех элементов
static void test_clear() {
    printf("TEST: hash_table_clear ... ");
    struct HashTable_t* ht = hash_table_create(10);
    for (int i = 0; i < 10; i++) {
        int* val = malloc(sizeof(int));
        *val = i * 100;
        hash_table_put(ht, i, val);
    }
    assert(hash_table_count(ht) == 10);
    hash_table_clear(ht);
    assert(hash_table_count(ht) == 0);
    assert(hash_table_get(ht, 5) == NULL);
    hash_table_free(ht);
    printf("PASSED\n");
}

// Отрицательные ключи
static void test_negative_keys() {
    printf("TEST: hash_table negative keys ... ");
    struct HashTable_t* ht = hash_table_create(10);
    int* v1 = malloc(sizeof(int)); *v1 = 100;
    int* v2 = malloc(sizeof(int)); *v2 = 200;
    hash_table_put(ht, -5, v1);
    hash_table_put(ht, -15, v2);
    assert(hash_table_count(ht) == 2);
    assert(*(int*)hash_table_get(ht, -5) == 100);
    assert(*(int*)hash_table_get(ht, -15) == 200);
    hash_table_free(ht);
    printf("PASSED\n");
}

// Автоматическое рехеширование при переполнении
static void test_resize() {
    printf("TEST: hash_table_resize (auto grow) ... ");
    struct HashTable_t* ht = hash_table_create(5);
    int old_size = hash_table_size(ht);
    for (int i = 0; i < 20; i++) {
        int* val = malloc(sizeof(int));
        *val = i * 10;
        hash_table_put(ht, i, val);
    }
    int new_size = hash_table_size(ht);
    assert(hash_table_count(ht) == 20);
    assert(new_size > old_size);
    for (int i = 0; i < 20; i++) {
        assert(*(int*)hash_table_get(ht, i) == i * 10);
    }
    hash_table_free(ht);
    printf("PASSED (size %d -> %d)\n", old_size, new_size);
}

// Создание с нулевым размером
static void test_zero_size() {
    printf("TEST: hash_table_create with zero size ... ");
    struct HashTable_t* ht = hash_table_create(0);
    assert(ht == NULL);
    printf("PASSED\n");
}

// Операции с NULL-таблицей
static void test_null_table() {
    printf("TEST: operations on NULL table ... ");
    hash_table_put(NULL, 1, NULL);
    void* val = hash_table_get(NULL, 1);
    assert(val == NULL);
    int removed = hash_table_remove(NULL, 1);
    assert(removed == 0);
    hash_table_free(NULL);
    printf("PASSED\n");
}

// Запуск всех тестов
int main() {
    printf("\n=== HASH TABLE TESTS ===\n\n");
    test_create();
    test_put_get();
    test_update();
    test_remove();
    test_collisions();
    test_remove_from_chain();
    test_clear();
    test_negative_keys();
    test_resize();
    test_zero_size();
    test_null_table();
    printf("\nALL HASH TABLE TESTS PASSED!\n");
    return 0;
}
