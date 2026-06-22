// ============================================================
// Двусвязный список для LRU-кеша
// ============================================================
// Особенности:
// - Голова (head) — самый свежий элемент
// - Хвост (tail) — самый старый элемент
// - Узлы (ListNode_t) инкапсулированы
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

struct ListNode_t {
    int key;
    void* value;
    struct ListNode_t* prev;
    struct ListNode_t* next;
};

struct List_t {
    struct ListNode_t* head;
    struct ListNode_t* tail;
    size_t size;
};

static struct ListNode_t* create_node(int key, void* value) {
    struct ListNode_t* node = (struct ListNode_t*)malloc(sizeof(struct ListNode_t));
    if (!node) return NULL;
    node->key = key;
    node->value = value;
    node->prev = NULL;
    node->next = NULL;
    return node;
}

static void free_node(struct ListNode_t* node) {
    if (!node) return;
    free(node);
}

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

struct List_t* list_create(void) {
    struct List_t* list = (struct List_t*)malloc(sizeof(struct List_t));
    if (!list) return NULL;
    list->head = NULL;
    list->tail = NULL;
    list->size = 0;
    return list;
}

void list_free(struct List_t* list) {
    if (!list) return;
    list_clear(list);
    free(list);
}

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
    free_node(tail);
    list->size--;
    return 1;
}

void list_move_to_front(struct List_t* list, struct ListNode_t* node) {
    if (!list || !node) return;
    if (node == list->head) return;
    if (list->head == list->tail) return;
    
    if (node->prev) {
        node->prev->next = node->next;
    }
    if (node->next) {
        node->next->prev = node->prev;
    }
    
    if (node == list->tail) {
        list->tail = node->prev;
        if (list->tail) {
            list->tail->next = NULL;
        }
    }
    
    node->prev = NULL;
    node->next = list->head;
    if (list->head) {
        list->head->prev = node;
    }
    list->head = node;
}

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
    free_node(node);
    list->size--;
}

struct ListNode_t* list_head(struct List_t* list) {
    return list ? list->head : NULL;
}

struct ListNode_t* list_tail(struct List_t* list) {
    return list ? list->tail : NULL;
}

size_t list_size(struct List_t* list) {
    return list ? list->size : 0;
}

int list_empty(struct List_t* list) {
    return list ? (list->size == 0) : 1;
}

void* list_get_value(struct ListNode_t* node) {
    return node ? node->value : NULL;
}

int list_get_key(struct ListNode_t* node) {
    return node ? node->key : -1;
}

void list_set_value(struct ListNode_t* node, void* value) {
    if (node) {
        node->value = value;
    }
}
struct ListNode_t* list_node_get_next(struct ListNode_t* node) {
    return node ? node->next : NULL;
}

struct ListNode_t* list_node_get_prev(struct ListNode_t* node) {
    return node ? node->prev : NULL;
}

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
