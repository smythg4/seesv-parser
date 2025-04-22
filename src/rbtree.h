#pragma once
#include <stdio.h>
#include <stdlib.h>

typedef struct RBTreeNode rbt_node_t;

//global nil
rbt_node_t *nil;
int nil_ref_count = 0;

typedef struct RBTreeNode {
    void* data;
    rbt_node_t *left;
    rbt_node_t *right;
    rbt_node_t *parent;
    bool is_red;
    void (*free_data)(void *); // function pointer to free_data subroutine
    void (*print_data)(void *); // function pointer to print_data subroutine
} rbt_node_t;

typedef struct RedBlackTree {
    rbt_node_t *root;
    int (*compare_nodes_data)(void *, void *); // function pointer to compare_nodes subroutine
} redblack_tree_t;

void init_nil_node(){
    nil = (rbt_node_t *)malloc(sizeof(rbt_node_t));
    nil->left = NULL;
    nil->right = NULL;
    nil->parent = NULL;
    nil->is_red = false; // nil nodes are always black
    nil->data = NULL;
    nil->free_data = NULL;
    nil->print_data = NULL;
}

rbt_node_t *init_rbt_node(void *data, void (*free_func)(void *), void (*print_func)(void *)) {
    rbt_node_t *new_node = (rbt_node_t *)malloc(sizeof(rbt_node_t));
    new_node->left = nil;
    new_node->right = nil;
    new_node->parent = nil;
    new_node->is_red = true; // new nodes are always red by default
    new_node->data = data;
    new_node->free_data = free_func;
    new_node->print_data = print_func;
    return new_node;
}

redblack_tree_t *init_rbt(int (*compare_func)(void*, void*)) {
    if (nil == NULL) {
        //initialize sentinel if not already done
        init_nil_node();
    }
    redblack_tree_t *tree = (redblack_tree_t *)malloc(sizeof(redblack_tree_t));
    tree->root = nil;
    nil_ref_count++;
    tree->compare_nodes_data = compare_func;
    return tree;
}

void rbt_left_rotate(redblack_tree_t *tree, rbt_node_t *node) {
    rbt_node_t *right_child = node->right;
    node->right = right_child->left;
    if( right_child->left != nil) {
        right_child->left->parent = node;
    }
    right_child->parent = node->parent;
    if(node->parent == nil) {
        tree->root = right_child;
    }
    else if (node == node->parent->left) {
        node->parent->left = right_child;
    }
    else {
        node->parent->right = right_child;
    }
    right_child->left = node;
    node->parent = right_child;
}

void rbt_right_rotate(redblack_tree_t *tree, rbt_node_t *node) {
    rbt_node_t *left_child = node->left;
    node->left = left_child->right;
    if( left_child->right != nil) {
        left_child->right->parent = node;
    }
    left_child->parent = node->parent;
    if(node->parent == nil) {
        tree->root = left_child;
    }
    else if (node == node->parent->left) {
        node->parent->left = left_child;
    }
    else {
        node->parent->right = left_child;
    }
    left_child->right = node;
    node->parent = left_child;
}

void rbt_insert_fixup(redblack_tree_t *tree, rbt_node_t *node) {
    rbt_node_t *current = node;
    rbt_node_t *parent = node->parent;
    rbt_node_t *uncle = parent->parent->left; // might be redundant
    while( parent->is_red ) {
        if(parent == parent->parent->left) {
            // if current's parent is a left child then the uncle is the parent's right child
            uncle = parent->parent->right;
            if( uncle->is_red ) {
                //with a red uncle and red parent
                parent->is_red = false;
                uncle->is_red = false;
                parent->parent->is_red = true;
                current = parent->parent;
                parent = current->parent;
            }
            else {
                //with a black uncle and a red parent
                if(current == parent->right) {
                    // if current is a right child
                    current = parent;
                    parent = current->parent;
                    rbt_left_rotate(tree, current);
                }
                parent->is_red = false;
                parent->parent->is_red = true;
                rbt_right_rotate(tree, parent->parent);
            }
        }
        else {
            // if current's parent is a right child then the uncle is the parent's left child
            uncle = parent->parent->left;
            if( uncle->is_red ) {
                //with a red uncle and red parent
                parent->is_red = false;
                uncle->is_red = false;
                parent->parent->is_red = true;
                current = parent->parent;
                parent = current->parent;
            }
            else {
                //with a black uncle and a red parent
                if(current == parent->left) {
                    // if current is a left child
                    current = parent;
                    parent = current->parent;
                    rbt_right_rotate(tree, current);
                }
                parent->is_red = false;
                parent->parent->is_red = true;
                rbt_left_rotate(tree, parent->parent);
            }
        }
    }
    tree->root->is_red = false;
}

void rbt_insert(redblack_tree_t *tree, void* data, void (*free_func)(void *), void (*print_func)(void *)) {
    rbt_node_t *new_node = init_rbt_node(data, free_func, print_func);

    if (tree->root == nil) {
        tree->root = new_node;
        new_node->is_red = false; // root is always black
        return;
    }

    //find insertion point
    rbt_node_t *current = tree->root;
    rbt_node_t *parent = nil;
    int (*compare_func)(void*,void*) = tree->compare_nodes_data;

    while( current != nil ){
        parent = current;
        if(compare_func(new_node->data,current->data) <0 ) {
            current = current->left;
        }
        else {
            current = current->right;
        }
    }

    //set the new nodes parent
    new_node->parent = parent;

    //insert the new node as appropriate child
    if( compare_func(new_node->data, parent->data) < 0) {
        parent->left = new_node;
    }
    else {
        parent->right = new_node;
    }
    rbt_insert_fixup(tree, new_node);
}

void rbt_transplant(redblack_tree_t *tree, rbt_node_t *u, rbt_node_t *v) {
    // this function leaves subtree u dangling. Make sure you either attach it somewhere or free the memory after.
    if(u->parent == nil) {
        tree->root = v;
    }
    else if (u == u->parent->left) {
        u->parent->left = v;
    }
    else {
        u->parent->right = v;
    }
    v->parent = u->parent;
}

void print_rbt_nodes_r(rbt_node_t *root) {
    if(root == nil) {
        return;
    }
    else {
        print_rbt_nodes_r(root->left);
        if(root->is_red) {
            printf("(RED) ");
        }
        else {
            printf("(BLACK) ");
        }
        root->print_data(root->data);
        print_rbt_nodes_r(root->right);
    }
}

void print_rbt(redblack_tree_t *tree) {
    if(tree->root == nil) {
        printf("The tree was empty.\n");
    }
    else {
        print_rbt_nodes_r(tree->root);
    }
}

void free_rbt_r(rbt_node_t *root) {
    if (root == nil) {
        return;
    }
    free_rbt_r(root->left);
    free_rbt_r(root->right);
    root->free_data(root->data);
    free(root);
}

void free_rbt(redblack_tree_t *tree) {
    if(tree == NULL) {
        return;
    }
    nil_ref_count--;
    free_rbt_r(tree->root);
    free(tree);
    if(nil_ref_count <= 0) {
        free(nil);
    }
}