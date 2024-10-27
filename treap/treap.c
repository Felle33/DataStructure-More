//
// Created by tommy on 22/10/24.
//

#include <stdlib.h>
#include <time.h>
#include "treap.h"

#define LEFT_CHILD(x) (x->parent->left == x)
#define IS_LEAF(x) (x->left == NULL && x->right == NULL)

static Treap *initialize_treap_node(int x);

static Treap *initialize_treap_node_minus_inf(int x);

static void rotateLeft(Treap *node);

static void rotateRight(Treap *node);

static void list_helper(Treap *T, int *buffer, int *idx_buffer);

static Treap *select_child_smallest_prio(Treap *T);

static int size = 0;

void initialize_treap() {
    srand(time(0));
}

/**
 * Return the minimum element of the treap
 * @param T the treap
 * @return the minimum element
 */
// TODO: NOT IMPLEMENT IF THE TREAP IS EMPTY
int minimum(Treap **T) {
    Treap *root = *T;
    while (root->left != NULL) {
        root = root->left;
    }

    return root->data;
}

/**
 * Return the maximum element of the treap
 * @param T the treap
 * @return the maximum element
 */
// TODO: NOT IMPLEMENT IF THE TREAP IS EMPTY
int maximum(Treap **T) {
    Treap *root = *T;
    while (root->right != NULL) {
        root = root->right;
    }

    return root->data;
}

/**
 * Returns a list of elements in the treap sorted in ascending order
 * @param T the treap
 * @return the list of elements
 */
int *list(Treap **T) {
    int *buffer = (int *) malloc(size * sizeof(int));
    if (!buffer) {
        //err
        exit(1);
    }
    int idx = 0;
    list_helper(*T, buffer, &idx);
    return buffer;
}

/**
 * Search the value x in the Treap T
 * @param T the treap
 * @param x the value to search
 * @return the node element if it is present, NULL otherwise
 */
Treap *search(Treap **T, int x) {
    Treap *root = *T;

    while (root != NULL) {
        if (root->data == x) return root;
        if (x > root->data) {
            root = root->right;
        } else {
            root = root->left;
        }
    }

    return NULL;
}

/**
 * Insert an element in the treap if it is not present
 * @param T the treap
 * @param node_to_insert the node to insert if it is wanted, NULL otherwise
 * @param x the element
 * @return true if the element is inserted, false otherwise
 */
bool insert(Treap **T, Treap *node_to_insert, int x) {
    if (!node_to_insert) {
        node_to_insert = initialize_treap_node(x);
    }
    Treap *insert_pos = NULL;
    Treap *root = *T;

    if (root == NULL) {
        *T = node_to_insert;
        size += 1;
        return true;
    }

    if (search(T, x)) return false;

    while (root != NULL) {
        insert_pos = root;
        if (x > root->data) {
            root = root->right;
        } else {
            root = root->left;
        }
    }

    node_to_insert->parent = insert_pos;
    if (x > insert_pos->data) {
        insert_pos->right = node_to_insert;
    } else {
        insert_pos->left = node_to_insert;
    }

    while (node_to_insert->parent->priority > node_to_insert->priority) {
        if (LEFT_CHILD(node_to_insert)) {
            rotateRight(node_to_insert->parent);
        } else {
            rotateLeft(node_to_insert->parent);
        }
    }

    size += 1;
    return true;
}

/**
 * Delete the element x in the treap T
 * @param T the treap
 * @param x the element to delete
 * @return true if it is deleted, false otherwise
 */
bool delete(Treap **T, int x) {
    Treap *node_to_delete = search(T, x);
    if (!node_to_delete) return false;

    while (!IS_LEAF(node_to_delete)) {
        Treap *child_smallest_prio = select_child_smallest_prio(node_to_delete);
        if (LEFT_CHILD(child_smallest_prio)) {
            rotateRight(node_to_delete);
        } else {
            rotateLeft(node_to_delete);
        }
    }

    if (LEFT_CHILD(node_to_delete)) {
        node_to_delete->parent->left = NULL;
    } else {
        node_to_delete->parent->right = NULL;
    }

    free(node_to_delete);
    size -= 1;
    return true;
}

Treap *union_treap(Treap **T1, Treap **T2) {
    int max_t1 = maximum(T1);
    int min_t2 = minimum(T2);

    if (max_t1 + 1 >= min_t2) return NULL;

    int k = max_t1 + 1;
    Treap *node_divisor = initialize_treap_node_minus_inf(k);
    Treap *root_T1 = *T1;
    Treap *root_T2 = *T2;
    node_divisor->left = root_T1;
    node_divisor->right = root_T2;

    root_T1->parent = node_divisor;
    root_T2->parent = node_divisor;

    Treap *T = node_divisor;
    delete(&T, k);
    return T;
}

void split_treap(Treap **T, int x, Treap **T1, Treap **T2) {
    Treap *node_x = search(T, x);

    if (node_x) {

    }

    Treap *node_minus_inf = initialize_treap_node_minus_inf(x);
    insert(T, node_minus_inf, x);

    if (node_x) {

    }

    *T1 = (*T)->left;
    *T2 = (*T)->right;
}

static Treap *initialize_treap_node(int x) {
    Treap *node = (Treap *) malloc(sizeof(Treap));
    node->priority = rand();
    node->data = x;
    node->left = NULL;
    node->right = NULL;
    node->parent = NULL;
    return node;
}

static Treap *initialize_treap_node_minus_inf(int x) {
    Treap *node = (Treap *) malloc(sizeof(Treap));
    node->priority = -1;
    node->data = x;
    node->left = NULL;
    node->right = NULL;
    node->parent = NULL;
    return node;
}

static void rotateLeft(Treap *node) {
    Treap *right_child = node->right;

    node->right = right_child->left;
    right_child->parent = node->parent;
    node->parent = right_child;
    right_child->left = node;
}

static void rotateRight(Treap *node) {
    Treap *left_child = node->left;

    node->left = left_child->right;
    left_child->parent = node->parent;
    node->parent = left_child;
    left_child->right = node;
}

static void list_helper(Treap *T, int *buffer, int *idx_buffer) {
    if (!T) return;
    list_helper(T->left, buffer, idx_buffer);
    int idx = *idx_buffer;
    buffer[idx++] = T->data;
    *idx_buffer = idx;
    list_helper(T, buffer, idx_buffer);
}

static Treap *select_child_smallest_prio(Treap *T) {
    if (!T->left) return T->right;
    if (!T->right) return T->left;

    return T->left->priority < T->right->priority ? T->left : T->right;
}