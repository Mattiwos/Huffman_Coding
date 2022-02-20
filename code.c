#include "code.h"

#include <assert.h>
#include <stdio.h>

Code code_init(void) {
    Code c;
    c.top = 0;
    for (int i = 0; i < MAX_CODE_SIZE; i++) {
        c.bits[i] = 0;
    }
    return c;
}

uint32_t code_size(Code *c) {
    assert(c);
    return c->top;
}

bool code_empty(Code *c) {
    assert(c);
    return c->top == 0;
}

bool code_full(Code *c) {
    assert(c);
    return c->top == ALPHABET;
}

bool code_push_bit(Code *c, uint8_t bit) {
    assert(c);
    if (!code_full(c)) {
        if (bit == 1) { //get bit of c at i
            c->bits[c->top / 8] |= (1 << (c->top % 8)); //set_bit(c->top,c->bits)
        } else if (bit == 0) {
            c->bits[c->top / 8] &= ~(1 << (c->top % 8)); //clr bit
        }

        c->top += 1;
        return true;
    }
    return false;
}

bool code_pop_bit(Code *c, uint8_t *bit) {
    assert(c);
    if (!code_empty(c)) {
        c->top -= 1;
        *bit = (c->bits[c->top / 8] & (1 << (c->top % 8))) >> (c->top % 8); //get bit
        return true;
    }
    return false;
}

void code_print(Code *c) {
    assert(c);
    for (uint32_t i = 0; i < c->top; i++) {
        printf("%hhu ", c->bits[i]);
    }
    printf("\n");
    return;
}
