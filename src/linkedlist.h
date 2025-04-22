#pragma once
#include <stdio.h>
#include <stdlib.h>

typedef struct Node node_t;

typedef struct Node {
    void* data;
    node_t *next;
    void (*free_data)(void *);
    void (*print_data)(void *);
} node_t;

typedef struct LinkedList {
    node_t *head;
} linked_list_t;

linked_list_t *init_list() {
    linked_list_t *list = (linked_list_t*)malloc(sizeof(linked_list_t));
    list->head = NULL;
    return list;
}

void print_list(linked_list_t *list) {
    if(list->head == NULL) {
        printf("The list was empty.\n");
    }
    else {
        node_t *current_node = list->head;
        while(current_node != NULL) {
            current_node->print_data(current_node->data);
            current_node = current_node->next;
        }
    }
}

void add_item(linked_list_t *list, 
                void *data,
                void (*free_func)(void *),
                void (*print_func)(void *)) {
    printf("Just entered the add_item subpoutine...\n");

    node_t *new_node = (node_t*)malloc(sizeof(node_t));
    new_node->data = data;
    new_node->free_data = free_func;
    new_node->print_data = print_func;
    new_node->next = NULL;

    if( list->head == NULL ) {
        list->head = new_node;
    }
    else {
        node_t *current_node = list->head;
        while( current_node->next != NULL ) {
            current_node = current_node->next;
        }
        current_node->next = new_node;
    }
}

void remove_item(linked_list_t *list,
                    void *data,
                    int (*compare_func)(void*, void*)) {
    if (list->head == NULL) {
        printf("The list was empty, nothing to remove.\n");
        return;
    }
    // check if the head is the element to remove
    if (compare_func(list->head->data, data) == 0) {
        node_t *temp = list->head;
        list->head = list->head->next;
        free(temp);
        return;
    }

    node_t *current_node = list->head;
    while(current_node->next != NULL) {
        if( compare_func(current_node->next->data, data) == 0 ) {
            // if the next item is the one to delete
            // create a temp pointer to that item
            // make the next pointer of the current node point to two items down
            node_t *temp = current_node->next;
            current_node->next = current_node->next->next;
            // free the data in the node to remove
            temp->free_data(temp->data);
            // free the node to remove
            free(temp);
            return;
        }
        current_node = current_node->next;
    }
    printf("Item not found in list.\n");
}

node_t *find_min(node_t *head,
                node_t *min_so_far,
                int (*compare_func)(void*,void*)) {
    if (head == NULL) {
        return min_so_far;
    }
    if( compare_func(head->data, min_so_far->data) < 0 ) {
        min_so_far = head;
    }
    return find_min(head->next, min_so_far, compare_func);
}

node_t *find_middle(node_t *head) {
    if (head == NULL) {
        return NULL;
    }

    node_t *slow = head;
    node_t *fast = head;

    while  (fast->next != NULL && fast->next->next != NULL) {
        slow = slow->next;
        fast = fast->next->next;
    }
    return slow;
}

node_t *merge(node_t *left, node_t *right, int (compare_func)(void*, void*)) {
    node_t dummy;
    node_t *current = &dummy;

    while( left != NULL && right != NULL) {
        if( compare_func(left->data,right->data) < 0 ) {
            current->next = left;
            left = left->next;
        }
        else {
            current->next = right;
            right = right->next;
        }
        current = current->next;
    }

    if( left != NULL ) {
        current->next = left;
    }
    else {
        current->next = right;
    }
    return dummy.next;
}

node_t *merge_sort_r(node_t *head, int (*compare_func)(void*, void*)) {
    if (head == NULL || head->next == NULL) {
        // if head is NULL or list is only one item
        return head;
    }

    // split the list in two
    node_t *middle = find_middle(head);
    node_t *second_half = middle->next;
    middle->next = NULL;

    //recursively sort each half
    node_t *first_half_sorted = merge_sort_r(head, compare_func);
    node_t *second_half_sorted = merge_sort_r(second_half, compare_func);

    //merge the sorted halves
    return merge(first_half_sorted, second_half_sorted, compare_func);
}

void merge_sort(linked_list_t *list, int (*compare_func)(void*, void*)) {
    list->head = merge_sort_r(list->head, compare_func);
}

void swap_nodes(node_t *node_a, node_t *node_b) {
    if( node_a == NULL || node_b == NULL ) {
        return;
    }
    void *temp = node_a->data;
    node_a->data = node_b->data;
    node_b->data = temp;
}

void selection_sort_r(node_t *head,
                        linked_list_t *list,
                        int (*compare_func)(void*,void*)) {
    if( head == NULL ) {
        return;
    }
    node_t *min_node = find_min(head, head, compare_func);
    if( head != min_node ) {
        swap_nodes(head, min_node);
    }

    selection_sort_r(head->next, list, compare_func);
}

void selection_sort(linked_list_t *list, int (*compare_func)(void*,void*)) {
    selection_sort_r(list->head, list, compare_func);
}

void free_list(linked_list_t *list) {
    node_t *current_node = list->head;
    while( current_node != NULL ) {
        node_t *next = current_node->next;
        current_node->free_data(current_node->data);
        free(current_node);
        current_node = next;
    }
    free(list);
}