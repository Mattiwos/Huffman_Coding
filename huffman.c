#include "huffman.h"

#include "code.h"
#include "defines.h"
#include "node.h"
#include "pq.h"
#include "stack.h"

#include <stdio.h>

Node *build_tree(uint64_t hist[static ALPHABET]) {
    PriorityQueue *q = pq_create(ALPHABET); //creates a priority queue
    for (uint64_t i = 0; i < ALPHABET; i++) { //loops through histogram
        if (hist[i] != 0) { //if frequency is greater than 1 then enqueue it
            enqueue(q, node_create(i, hist[i]));
        }
    }
    //tree
    Node *left = NULL;
    Node *right = NULL;
    while (pq_size(q) > 1) { //loop through priority queue untill only one node left
        dequeue(q, &left); //get left node
        dequeue(q, &right); //get right node
        enqueue(q, node_join(left, right)); //join them together and enqueue them
    }
    Node *n = NULL;
    dequeue(q, &n); //remove root from pq
    pq_delete(&q); //delete priority queue
    return n; //return root
}

void build(Node *root, Code table[static ALPHABET], Code c) {
    uint8_t buf; ///discards popped code
    if (root->right == NULL && root->left == NULL) { //check if root is a leaf
        table[root->symbol] = c; //save the code if so
    } else {
        if (root->left != NULL) { //if root->exsists
            code_push_bit(&c, 0); //push 0 to the code
            build(root->left, table, c); //explore left node
            //pop 0
            code_pop_bit(&c, &buf); //pop 0 returned from exploring left node
        }
        if (root->right != NULL) {
            code_push_bit(&c, 1); //push 1 to the code to go right
            build(root->right, table, c); //explore right node
            //pop 1
            code_pop_bit(&c, &buf); //finished exploring left node
        }
    }
    return;
}

void build_codes(Node *root, Code table[static ALPHABET]) {
    Code c = code_init(); //initalized new code
    build(root, table, c); //calls recursive to build code
    return;
}

Node *rebuild_tree(uint16_t nbytes, uint8_t tree[static nbytes]) { //rebuilds tree from tree dump
    Stack *s = stack_create(nbytes); //creates a stack
    for (int i = 0; i < nbytes; i++) { //loops through the tree dump
        if (tree[i] == (uint8_t) 'L') { //if tree is L
            stack_push(s, node_create(tree[i + 1], 0)); //no clue about frequency and doesn't matter
            i += 1; //skip next charcter
        } else if (tree[i] == (uint8_t) 'I') { //encountered interior node
            Node *left = NULL;
            Node *right = NULL;
            stack_pop(s, &right); //right node
            stack_pop(s, &left); //left node
            stack_push(s, node_join(left, right)); //join them and push them to the stack
        }
    }
    Node *n = NULL;
    stack_pop(s, &n); //pop root node
    stack_delete(&s); //delete stack
    return n; //return root
}

void delete_tree(Node **root) {
    if (*root) { //check if root is valid
        delete_tree(&(*root)->left); //delete left node recursively
        delete_tree(&(*root)->right); //delete right node recursively
        node_delete(root);
    }
    return;
}
