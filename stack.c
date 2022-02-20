#include "stack.h"

#include "node.h"

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct Stack {
    uint32_t top;
    uint32_t capacity;
    Node **items;
} Stack;

Stack *stack_create(uint32_t capacity) {
    Stack *s = (Stack *) malloc(sizeof(Stack));
    assert(s);
    s->capacity = capacity;
    s->top = 0;
    s->items = (Node **) calloc(capacity, sizeof(Node *));
    if (!s->items) {
        free(s);
        s = NULL;
    }
    return s;
}

void stack_delete(Stack **s) {
    assert(*s);
    free((*s)->items);
    free((*s));
    *s = NULL;
    return;
}

bool stack_empty(Stack *s) {
    assert(s);
    return s->top == 0;
}
bool stack_full(Stack *s) {
    assert(s);
    return s->top == s->capacity;
}

uint32_t stack_size(Stack *s) {
    assert(s);
    return s->top;
}

bool stack_push(Stack *s, Node *n) {
    assert(s);
    assert(n);
    if (!stack_full(s)) {
        s->items[s->top] = n;
        s->top += 1;
        return true;
    }
    return false;
}

bool stack_pop(Stack *s, Node **n) {
    assert(s);
    assert(n);
    if (!stack_empty(s)) {
        s->top -= 1;
        *n = s->items[s->top];
        return true;
    }
    return false;
}
void stack_print(Stack *s) {
    assert(s);
    for (uint32_t i = 0; i < s->top; i++) {
        node_print(s->items[i]);
    }
    printf("\n");
}
