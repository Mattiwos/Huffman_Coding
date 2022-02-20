#include "node.h"

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

Node *node_create(uint8_t symbol, uint64_t frequency) {
    Node *n = (Node *) malloc(sizeof(Node));
    assert(n);
    n->symbol = symbol;
    n->frequency = frequency;
    n->right = NULL;
    n->left = NULL;
    return n;
}

void node_delete(Node **n) {
    assert(n);
    assert(*n);
    free(*n);
    *n = NULL;
    return;
}

Node *node_join(Node *left, Node *right) {
    Node *n = node_create((uint8_t) '$', left->frequency + right->frequency);
    n->left = left;
    n->right = right;
    return n;
}

void node_print(Node *n) {
    assert(n);
    printf("%c: Freq: %lu\n", (char) n->symbol, n->frequency);
    if (n->left) {
        printf("Left: ");
        node_print(n->left);
    }
    if (n->right) {
        printf("Right: ");
        node_print(n->right);
    }
    return;
}
