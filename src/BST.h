#pragma once
#include <stdio.h>
#include <stdlib.h>

typedef struct TreeNode tree_node_t;

typedef struct TreeNode {
    void* data;
    tree_node_t *left;
    tree_node_t *right;
    void (*free_data)(void *); // function pointer to free_data subroutine
    void (*print_data)(void *); // function pointer to print_data subroutine
} tree_node_t;

typedef struct BST {
    tree_node_t *root;
    int (*compare_nodes_data)(void *, void *); // function pointer to compare_nodes subroutine
} bst_t;

bst_t *init_bst(int (*compare_func)(void*, void*)) {
    bst_t *tree = (bst_t *)malloc(sizeof(bst_t));
    tree->root = NULL;
    tree->compare_nodes_data = compare_func;
    return tree;
}

tree_node_t *bst_insert_r(tree_node_t *root, tree_node_t *new_node, int (*compare_func)(void*, void*)) {
    if ( root == NULL ){
        return new_node;
    }
    else if( compare_func(new_node->data, root->data) < 0) {
        root->left = bst_insert_r(root->left, new_node, compare_func);
    }
    else {
        root->right = bst_insert_r(root->right, new_node, compare_func);
    }
    return root;
}

void bst_insert(bst_t *tree, void* data, void (*free_func)(void *), void (*print_func)(void *)) {
    int (*compare_func)(void*, void*) = tree->compare_nodes_data;
    tree_node_t *new_node = (tree_node_t *)malloc(sizeof(tree_node_t));
    new_node->data = data;
    new_node->free_data = free_func;
    new_node->print_data = print_func;
    new_node->left = NULL;
    new_node->right = NULL;
    
    tree->root = bst_insert_r(tree->root, new_node, compare_func);
}

tree_node_t *bst_find_parent(tree_node_t *root, void *key, int (compare_func)(void*, void*)) {
    // consider replacing this function by maintaining a parent pointer in the node structure.
    if (root == NULL) {
        return NULL;
    }
    else if (root->left != NULL && compare_func(root->left->data, key) == 0) {
        return root;
    }
    else if (root->right != NULL && compare_func(root->right->data, key) == 0) {
        return root;
    }
    else if (compare_func(root->data, key) < 0) {
        return bst_find_parent(root->right, key, compare_func);
    }
    else {
        return bst_find_parent(root->left, key, compare_func);
    }
}

tree_node_t *bst_find_min(tree_node_t *node) {
    if (node == NULL) {
        return NULL;
    }
    else if (node->left == NULL) {
        return node;
    }
    else {
        return bst_find_min(node->left);
    }
}

bool bst_remove(bst_t *tree, void *key) {    
    if ( tree==NULL || tree->root == NULL) {
        return false;
    }
    int (*compare_func)(void*, void*) = tree->compare_nodes_data;

    tree_node_t *current = tree->root;
    tree_node_t *parent = NULL;

    while(current != NULL) {
        int result = compare_func(key, current->data);

        if (result == 0) {
            //we found a match!
            break;
        }
        parent = current;
        if(result < 0) {
            current = current->left;
        }
        else {
            current = current->right;
        }
    }

    if ( current == NULL) {
        // no match found.
        return false;
    }

    //case 1: node to remove has no children
    if(current->left == NULL && current->right == NULL) {
        if (parent == NULL) {
            tree->root = NULL;
        }
        else if (parent->left == current) {
            parent->left = NULL;
        }
        else {
            parent->right = NULL;
        }
        current->free_data(current->data);
        free(current);
    }

    //case 2: node to remove has only a right child
    else if (current->left == NULL) {
        if(parent == NULL) {
            tree->root = current->right;
        }
        else if (parent->left == current) {
            parent->left = current->right;
        }
        else {
            parent->right = current->right;
        }
        current->free_data(current->data);
        free(current);
    }

    //case 3: node to remove has only a left child
    else if(current->right == NULL) {
        if(parent == NULL) {
            tree->root = current->left;
        }
        else if(parent->left == current) {
            parent->left = current->left;
        }
        else {
            parent->right = current->left;
        }

        current->free_data(current->data);
        free(current);
    }

    //case 4: node to remove has 2 children
    else {
        // find in-order successor. The smallest node in the right subtree
        tree_node_t *successor = bst_find_min(current->right);
        tree_node_t *successor_parent;
        
        if(current->right == successor) {
            successor_parent = current;
        }
        else {
            successor_parent = bst_find_parent(tree->root, successor->data, compare_func);
        }
        //save a copy of the successor's data
        void *successor_data = successor->data;

        //remove the successor (which has at most one child)
        if (successor_parent->left == successor) {
            successor_parent->left = successor->right;
        }
        else {
            successor_parent->right = successor->right;
        }

        current->free_data(current->data);
        current->data = successor_data;

        free(successor);
    }
    return true;
} 

void print_bst_nodes_r(tree_node_t *root) {
    if( root == NULL) {
        return; // do nothing
    }
    else {
        print_bst_nodes_r(root->left);
        root->print_data(root->data);
        print_bst_nodes_r(root->right);
    }
}
void print_bst(bst_t *tree) {
    if(tree->root == NULL) {
        printf("The tree was empty.\n");
    }
    else {
        print_bst_nodes_r(tree->root);
    }
}

void free_bst_r(tree_node_t *root) {
    if (root == NULL) {
        return;
    }
    free_bst_r(root->left);
    free_bst_r(root->right);
    root->free_data(root->data);
    free(root);
}

void free_bst(bst_t *tree) {
    if(tree == NULL) {
        return;
    }
    free_bst_r(tree->root);
    free(tree);
}
