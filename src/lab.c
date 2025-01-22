#include "lab.h"
#include <stdio.h>

// Initialize a new list
list_t *list_init(void (*destroy_data)(void *), int (*compare_to)(const void *, const void *)) {
    list_t *list = (list_t *)malloc(sizeof(list_t));
    if (!list) return NULL;

    list->destroy_data = destroy_data;
    list->compare_to = compare_to;
    list->size = 0;

    list->head = (node_t *)malloc(sizeof(node_t));
    if (!list->head) {
        free(list);
        return NULL;
    }

    list->head->data = NULL;
    list->head->next = list->head;
    list->head->prev = list->head;

    return list;
}

// Destroy the list and free all associated memory
void list_destroy(list_t **list_ptr) {
    if (!list_ptr || !(*list_ptr)) return;

    list_t *list = *list_ptr;
    node_t *current = list->head->next;

    while (current != list->head) {
        node_t *next = current->next;
        if (list->destroy_data) {
            list->destroy_data(current->data);
        }
        free(current);
        current = next;
    }

    free(list->head);
    free(list);
    *list_ptr = NULL;
}

// Add data to the front of the list
list_t *list_add(list_t *list, void *data) {
    if (!list) return NULL;

    node_t *new_node = (node_t *)malloc(sizeof(node_t));
    if (!new_node) return NULL;

    new_node->data = data;
    new_node->next = list->head->next;
    new_node->prev = list->head;

    list->head->next->prev = new_node;
    list->head->next = new_node;

    list->size++;
    return list;
}

// Remove a node at a specific index
void *list_remove_index(list_t *list, size_t index) {
    if (!list || index >= list->size) return NULL;

    node_t *current = list->head->next;
    for (size_t i = 0; i < index; i++) {
        current = current->next;
    }

    void *data = current->data;
    current->prev->next = current->next;
    current->next->prev = current->prev;

    free(current);
    list->size--;

    return data;
}

// Find the index of the first occurrence of data
int list_indexof(list_t *list, void *data) {
    if (!list || !list->compare_to) return -1;

    node_t *current = list->head->next;
    int index = 0;

    while (current != list->head) {
        if (list->compare_to(current->data, data) == 0) {
            return index;
        }
        current = current->next;
        index++;
    }

    return -1;
}

