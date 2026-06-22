#ifndef LIST_H
#define LIST_H

#include <stddef.h>

struct ListNode_t;
struct List_t;

struct List_t* list_create(void);
void list_free(struct List_t* list);

struct ListNode_t* list_push_front(struct List_t* list, int key, void* value);
int list_pop_back(struct List_t* list);
void list_move_to_front(struct List_t* list, struct ListNode_t* node);
void list_remove_node(struct List_t* list, struct ListNode_t* node);

struct ListNode_t* list_head(struct List_t* list);
struct ListNode_t* list_tail(struct List_t* list);
size_t list_size(struct List_t* list);
void* list_get_value(struct ListNode_t* node);
void list_set_value(struct ListNode_t* node, void* value);
int list_get_key(struct ListNode_t* node);
struct ListNode_t* list_node_get_next(struct ListNode_t* node);
struct ListNode_t* list_node_get_prev(struct ListNode_t* node);
int list_empty(struct List_t* list);
void list_clear(struct List_t* list);
void list_print(struct List_t* list);
#endif

