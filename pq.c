#include "pq.h"

#include "node.h"

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct PriorityQueue {
    uint32_t head;
    uint32_t tail;
    uint32_t size;
    uint32_t capacity;
    Node **items;
} PriorityQueue;

PriorityQueue *pq_create(uint32_t capacity) {
    PriorityQueue *q = (PriorityQueue *) malloc(sizeof(PriorityQueue));
    if (q) {
        q->head = 0;
        q->tail = 0;
        q->size = 0;
        q->capacity = capacity;
        q->items = (Node **) calloc(capacity, sizeof(Node *));
        if (!q->items) {
            free(q);
            q = NULL;
        }
    }
    return q;
}

void pq_delete(PriorityQueue **q) {
    if (*q && (*q)->items) {
        free((*q)->items);
        free((*q));
        *q = NULL;
    }
    return;
}

bool pq_empty(PriorityQueue *q) {
    assert(q);
    return q->size == 0;
}

bool pq_full(PriorityQueue *q) {
    assert(q);
    return q->size == q->capacity;
}

uint32_t pq_size(PriorityQueue *q) {
    assert(q);
    return q->size;
}

bool enqueue(PriorityQueue *q, Node *n) { //needs improvement and testing
    assert(q);
    assert(n);
    if (!pq_full(q)) {
        //perform inserction
        q->items[q->tail] = n; //adds n to queue
        uint32_t i = q->tail; //beginning tail
        while (i != q->head) { //going down tail
            if (q->items[i]->frequency
                < q->items[(i - 1) % q->capacity]->frequency) { //check if curr frequency is less
                //swap
                Node *temp = q->items[i];
                q->items[i] = q->items[(i - 1) % q->capacity];
                q->items[(i - 1) % q->capacity] = temp;
                i = (i - 1) % q->capacity; //decrease i - index
                continue;
            }
            break;
        }
        q->tail = (q->tail + 1) % q->capacity;
        q->size += 1;
        return true;
    }
    return false;
}

bool dequeue(PriorityQueue *q, Node **n) {
    assert(q);
    if (!pq_empty(q)) { //dequeues node
        *n = q->items[q->head];
        q->head = (q->head + 1) % q->capacity;
        q->size -= 1;
        return true;
    }
    return false;
}

void pq_print(PriorityQueue *q) {
    if (q) {
        printf("Printing PriorityQueue\n");
        printf("Size: %u\n", q->size);
        for (uint32_t i = 0; i < q->size; i++) {
            node_print(q->items[((q->head + i) % q->capacity)]);
        }
    }
    return;
}
