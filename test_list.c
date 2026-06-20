```c
// ============================================================
// Тесты для двусвязного списка
// ============================================================
// Набор тестов покрывает все основные операции со списком.
//
// УПРАВЛЕНИЕ ПАМЯТЬЮ:
// Список НЕ освобождает value.
// Пользователь сам выделяет и освобождает память.
// В тестах мы вручную освобождаем все выделенные данные.
//
// Тесты:
// 1. Создание и уничтожение списка
// 2. Добавление элементов в голову
// 3. Перемещение существующих узлов в голову
// 4. Удаление элементов из хвоста
// 5. Удаление произвольных узлов
// 6. Очистка списка
// 7. Работа с одним элементом
// 8. Работа с двумя элементами
// 9. Хранение динамических значений (строки)
// ============================================================

#include "list.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

// Тест 1: Создание и уничтожение списка
static void test_create() {
    printf("TEST: list_create ... ");
    struct List_t* list = list_create();
    assert(list != NULL);
    assert(list_size(list) == 0);
    assert(list_empty(list) == 1);
    assert(list_head(list) == NULL);
    assert(list_tail(list) == NULL);
    list_free(list);
    printf("PASSED\n");
}

// Тест 2: Добавление элементов в голову
static void test_push_front() {
    printf("TEST: list_push_front ... ");
    struct List_t* list = list_create();
    int* a = malloc(sizeof(int)); *a = 100;
    int* b = malloc(sizeof(int)); *b = 200;
    int* c = malloc(sizeof(int)); *c = 300;
    list_push_front(list, 1, a);
    list_push_front(list, 2, b);
    list_push_front(list, 3, c);
    assert(list_size(list) == 3);
    assert(list_head(list)->key == 3);
    assert(list_tail(list)->key == 1);
    free(a);
    free(b);
    free(c);
    list_free(list);
    printf("PASSED\n");
}

// Тест 3: Перемещение существующих узлов в голову
static void test_move_to_front() {
    printf("TEST: list_move_to_front ... ");
    struct List_t* list = list_create();
    int* a = malloc(sizeof(int)); *a = 100;
    int* b = malloc(sizeof(int)); *b = 200;
    int* c = malloc(sizeof(int)); *c = 300;
    struct ListNode_t* n1 = list_push_front(list, 1, a);
    list_push_front(list, 2, b);
    list_push_front(list, 3, c);
    
    list_move_to_front(list, n1);
    assert(list_head(list)->key == 1);
    assert(list_tail(list)->key == 2);
    list_move_to_front(list, list_tail(list));
    assert(list_head(list)->key == 2);
    assert(list_tail(list)->key == 3);
    
    free(a);
    free(b);
    free(c);
    list_free(list);
    printf("PASSED\n");
}

// Тест 4: Удаление элементов из хвоста
static void test_pop_back() {
    printf("TEST: list_pop_back ... ");
    struct List_t* list = list_create();
    int* a = malloc(sizeof(int)); *a = 100;
    int* b = malloc(sizeof(int)); *b = 200;
    int* c = malloc(sizeof(int)); *c = 300;
    list_push_front(list, 1, a);
    list_push_front(list, 2, b);
    list_push_front(list, 3, c);
    assert(list_size(list) == 3);
    int popped = list_pop_back(list);
    assert(popped == 1);
    assert(list_size(list) == 2);
    assert(list_tail(list)->key == 2);
    popped = list_pop_back(list);
    assert(popped == 1);
    assert(list_size(list) == 1);
    assert(list_head(list)->key == 3);
    assert(list_tail(list)->key == 3);
    popped = list_pop_back(list);
    assert(popped == 1);
    assert(list_size(list) == 0);
    assert(list_head(list) == NULL);
    assert(list_tail(list) == NULL);
    popped = list_pop_back(list);
    assert(popped == 0);
    free(a);
    free(b);
    free(c);
    list_free(list);
    printf("PASSED\n");
}

// Тест 5: Удаление произвольных узлов
static void test_remove_node() {
    printf("TEST: list_remove_node ... ");
    struct List_t* list = list_create();
    int* a = malloc(sizeof(int)); *a = 100;
    int* b = malloc(sizeof(int)); *b = 200;
    int* c = malloc(sizeof(int)); *c = 300;
    struct ListNode_t* n1 = list_push_front(list, 1, a);
    struct ListNode_t* n2 = list_push_front(list, 2, b);
    struct ListNode_t* n3 = list_push_front(list, 3, c);
    assert(list_size(list) == 3);
    list_remove_node(list, n2);
    assert(list_size(list) == 2);
    assert(list_head(list) == n3);
    assert(list_tail(list) == n1);
    assert(n3->next == n1);
    assert(n1->prev == n3);
    list_remove_node(list, list_head(list));
    assert(list_size(list) == 1);
    assert(list_head(list) == n1);
    assert(list_tail(list) == n1);
    list_remove_node(list, list_head(list));
    assert(list_size(list) == 0);
    assert(list_head(list) == NULL);
    assert(list_tail(list) == NULL);
    free(a);
    free(b);
    free(c);
    list_free(list);
    printf("PASSED\n");
}

// Тест 6: Очистка списка
static void test_clear() {
    printf("TEST: list_clear ... ");
    struct List_t* list = list_create();
    int* a = malloc(sizeof(int)); *a = 100;
    int* b = malloc(sizeof(int)); *b = 200;
    int* c = malloc(sizeof(int)); *c = 300;
    list_push_front(list, 1, a);
    list_push_front(list, 2, b);
    list_push_front(list, 3, c);
    assert(list_size(list) == 3);
    list_clear(list);
    assert(list_size(list) == 0);
    assert(list_empty(list) == 1);
    assert(list_head(list) == NULL);
    assert(list_tail(list) == NULL);
    free(a);
    free(b);
    free(c);
    list_free(list);
    printf("PASSED\n");
}

// Тест 7: Работа с одним элементом
static void test_single_element() {
    printf("TEST: single element operations ... ");
    struct List_t* list = list_create();
    int* a = malloc(sizeof(int)); *a = 100;
    list_push_front(list, 1, a);
    assert(list_head(list) == list_tail(list));
    assert(list_head(list)->key == 1);
    list_move_to_front(list, list_head(list));
    assert(list_head(list)->key == 1);
    list_pop_back(list);
    assert(list_size(list) == 0);
    assert(list_head(list) == NULL);
    assert(list_tail(list) == NULL);
    free(a);
    list_free(list);
    printf("PASSED\n");
}

// Тест 8: Работа с двумя элементами
static void test_two_elements() {
    printf("TEST: two elements operations ... ");
    struct List_t* list = list_create();
    int* a = malloc(sizeof(int)); *a = 100;
    int* b = malloc(sizeof(int)); *b = 200;
    list_push_front(list, 1, a);
    list_push_front(list, 2, b);
    assert(list_head(list)->next == list_tail(list));
    assert(list_tail(list)->prev == list_head(list));
    list_move_to_front(list, list_tail(list));
    assert(list_head(list)->key == 1);
    assert(list_tail(list)->key == 2);
    list_remove_node(list, list_head(list));
    assert(list_size(list) == 1);
    assert(list_head(list)->key == 2);
    assert(list_tail(list)->key == 2);
    free(a);
    free(b);
    list_free(list);
    printf("PASSED\n");
}

// Тест 9: Хранение динамических значений (строки)
static void test_dynamic_values() {
    printf("TEST: dynamic values (strings) ... ");
    struct List_t* list = list_create();
    char* s1 = strdup("Hello");
    char* s2 = strdup("World");
    char* s3 = strdup("!!!");
    list_push_front(list, 1, s1);
    list_push_front(list, 2, s2);
    list_push_front(list, 3, s3);
    assert(list_size(list) == 3);
    assert(strcmp((char*)list_head(list)->value, "!!!") == 0);
    assert(strcmp((char*)list_tail(list)->value, "Hello") == 0);
    free(s1);
    free(s2);
    free(s3);
    list_free(list);
    printf("PASSED\n");
}

int main() {
    printf("\n=== LIST TESTS ===\n\n");
    test_create();
    test_push_front();
    test_move_to_front();
    test_pop_back();
    test_remove_node();
    test_clear();
    test_single_element();
    test_two_elements();
    test_dynamic_values();
    printf("\nALL LIST TESTS PASSED!\n");
    return 0;
}
```
