//
// Created by tommy on 22/10/24.
//
#include <stdbool.h>

#ifndef TREAP_TREAP_H
#define TREAP_TREAP_H

typedef struct treap {
    struct treap *left;
    struct treap *right;
    struct treap *parent;
    int data;
    int priority;
} Treap;

void initialize_treap();

Treap *search(Treap **T, int x);

bool insert(Treap **T, Treap *node_to_insert, int x);

bool delete(Treap **T, int x);

int minimum(Treap **T);

int maximum(Treap **T);

int *list(Treap **T);

Treap *union_treap(Treap **T1, Treap **T2);

void split_treap(Treap **T, int x, Treap **T1, Treap **T2);

#endif //TREAP_TREAP_H
