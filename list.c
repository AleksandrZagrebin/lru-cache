// ============================================================
// Двусвязный список для LRU-кеша
// ============================================================
// Особенности:
// - Голова (head) — самый свежий элемент
// - Хвост (tail) — самый старый элемент
// - Узлы (ListNode_t) открыты для прямого доступа из кеша
// - Список (List_t) инкапсулирован (скрыт от пользователя)
// - Все операции выполняются за O(1)
// - Значения хранятся как void* — можно хранить любые данные
//
// УПРАВЛЕНИЕ ПАМЯТЬЮ:
// Список создаёт узлы (ListNode_t) и освобождает их.
// Список НЕ освобождает value — это ответственность пользователя.
// Пользователь сам выделяет и освобождает данные.
// ============================================================

#include "list.h"
#include <stdio.h>
#include <stdlib.h>

// Структура списка (скрыта от пользователя)
struct List_t {
    struct ListNode_t* head;   // указатель на голову (самый свежий)
    struct ListNode_t* tail;   // указатель на хвост (самый старый)
    size_t size;               // количество элементов в списке
};

// Создание нового узла
static struct ListNode_t* create_node(int key, void* value) {
    struct ListNode_t* node = (struct ListNode_t*)malloc(sizeof(struct ListNode_t));
    if (!node) return NULL;
    node->key = key;
    node->value = value;
    node->prev = NULL;
    node->next = NULL;
    return node;
}

// Освобождение узла (освобождает только сам узел, value не трогает)
static void free_node(struct ListNode_t* node) {
    if (!node) return;
    free(node);  // освобождаем только узел, value остаётся пользователю
}

// Очистка списка (удаляет все узлы, НЕ освобождает value)
void list_clear(struct List_t* list) {
    if (!list) return;
    struct ListNode_t* current = list->head;
    while (current) {
        struct ListNode_t* next = current->next;
        free_node(current);
        current = next;
    }
    list->head = NULL;
    list->tail = NULL;
    list->size = 0;
}

// Создание пустого списка
struct List_t* list_create(void) {
    struct List_t* list = (struct List_t*)malloc(sizeof(struct List_t));
    if (!list) return NULL;
    list->head = NULL;
    list->tail = NULL;
    list->size = 0;
    return list;
}

// Полное освобождение списка (НЕ освобождает value)
void list_free(struct List_t* list) {
    if (!list) return;
    list_clear(list);
    free(list);
}

// Добавление элемента в голову (самый свежий)
struct ListNode_t* list_push_front(struct List_t* list, int key, void* value) {
    if (!list) return NULL;
    struct ListNode_t* node = create_node(key, value);
    if (!node) return NULL;
    if (list->head == NULL) {
        list->head = node;
        list->tail = node;
    } else {
        node->next = list->head;
        list->head->prev = node;
        list->head = node;
    }
    list->size++;
    return node;
}

// Удаление элемента из хвоста (НЕ освобождает value)
int list_pop_back(struct List_t* list) {
    if (!list || list->head == NULL) return 0;
    struct ListNode_t* tail = list->tail;
    if (list->head == list->tail) {
        list->head = NULL;
        list->tail = NULL;
    } else {
        list->tail = tail->prev;
        list->tail->next = NULL;
    }
    free_node(tail);  // освобождаем только узел, value не трогаем
    list->size--;
    return 1;
}

// Перемещение узла в голову (используется при cache hit)
void list_move_to_front(struct List_t* list, struct ListNode_t* node) {
    if (!list || !node) return;
    if (node == list->head) return;
    if (list->head == list->tail) return;
    
    // Отвязываем узел от текущей позиции
    if (node->prev) {
        node->prev->next = node->next;
    }
    if (node->next) {
        node->next->prev = node->prev;
    }
    
    // Если узел был хвостом, обновляем хвост
    if (node == list->tail) {
        list->tail = node->prev;
        if (list->tail) {
            list->tail->next = NULL;
        }
    }
    
    // Вставляем в голову
    node->prev = NULL;
    node->next = list->head;
    if (list->head) {
        list->head->prev = node;
    }
    list->head = node;
}

// Удаление произвольного узла (НЕ освобождает value)
void list_remove_node(struct List_t* list, struct ListNode_t* node) {
    if (!list || !node) return;
    if (node->prev) {
        node->prev->next = node->next;
    }
    if (node->next) {
        node->next->prev = node->prev;
    }
    if (node == list->head) {
        list->head = node->next;
    }
    if (node == list->tail) {
        list->tail = node->prev;
    }
    free_node(node);  // освобождаем только узел, value не трогаем
    list->size--;
}

// Получение головы списка (самый свежий)
struct ListNode_t* list_head(struct List_t* list) {
    return list ? list->head : NULL;
}

// Получение хвоста списка (самый старый)
struct ListNode_t* list_tail(struct List_t* list) {
    return list ? list->tail : NULL;
}

// Получение количества элементов
size_t list_size(struct List_t* list) {
    return list ? list->size : 0;
}

// Проверка, пуст ли список
int list_empty(struct List_t* list) {
    return list ? (list->size == 0) : 1;
}

// Отладочный вывод содержимого списка
void list_print(struct List_t* list) {
    if (!list || list->head == NULL) {
        printf("[List] empty\n");
        return;
    }
    printf("[List] size=%zu: ", list->size);
    struct ListNode_t* current = list->head;
    while (current) {
        printf("[%d:%p]", current->key, (void*)current->value);
        if (current->next) printf(" <-> ");
        current = current->next;
    }
    printf("\n");
}
