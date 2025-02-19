#include <stdio.h>
#include "treap.h"

void print_treap(Treap *T, int space) {
    if(!T) return;

    space += 10;

    print_treap(T->right, space);

    printf("\n");
    for(int i = 10; i < space; i++) {
        printf(" ");
    }
    printf("%d (%d)\n", T->data, T->priority);

    print_treap(T->left, space);
}

int main(void) {

    Treap *T = NULL;
    initialize_treap();
    insert(&T, NULL, 10);
    insert(&T, NULL, 20);
    insert(&T, NULL, 30);
    insert(&T, NULL, 40);
    insert(&T, NULL, 50);
    insert(&T, NULL, 60);
    insert(&T, NULL, 70);
    insert(&T, NULL, 80);

    print_treap(T, 0);

    return 0;
}
