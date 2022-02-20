#include "io.h"

#include "defines.h"

#include <fcntl.h>
#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

uint64_t bytes_read = 0;
uint64_t bytes_written = 0;

int read_bytes(int infile, uint8_t *buf, int nbytes) {
    int total = 0; //keeps track of number of bytes read
    int bytes = 0; //keeps track of bytes read by 1 call of read
    while ((bytes = read(infile, buf, nbytes - total))
           > 0) { //read as long as the number read is greater than 0
        if (bytes == -1) { //print erro
            printf("error in read_bytes\n");
        }
        total += bytes;
        if (total == nbytes) { //check if everything have beeen read
            break;
        }
    }
    bytes_read += total; //keeps track of overall number of btyes read
    return total; //return total read this instance
}

int write_bytes(int outfile, uint8_t *buf, int nbytes) {
    int total = 0; //keeps track of number of bytes read
    int bytes = 0; //keeps track of bytes read by 1 call of read
    while ((bytes = write(outfile, buf, nbytes - total)) > 0) {
        if (bytes == -1) { //print erro
            printf("error in read_write\n");
        }
        total += bytes;
        if (total == nbytes) { //check if everything have beeen read
            break;
        }
    }
    bytes_written += total; //keeps track of overall number of bytes written
    return total; //return number of total written this instance
}

static uint8_t buffer[BLOCK];
static int bitindex = 0;
static int lastbit = -1;

bool read_bit(int infile, uint8_t *bit) {
    if (bitindex == 0) { //if the first time bit is called then it fills it
        int bread = read_bytes(infile, buffer, BLOCK);
        if (bread < BLOCK) { //if bytes read is less
            lastbit = bread * 8 + 1; //saves last bit
        }
    }
    *bit = (buffer[bitindex / 8] & (1 << (bitindex % 8))) >> (bitindex % 8); //get bit from buffer
    bitindex += 1; //increment index
    if (bitindex == 8 * BLOCK) { //check if all bits have been read
        bitindex = 0; //reset bitindex
    }
    if (bitindex == lastbit) { //all bytes/bits have been read in the file
        printf("last bit\n");
        return false;
    }
    return true;
}
static uint8_t bufcode[BLOCK] = { 0 };
static int index = 0;

void write_code(int outfile, Code *c) {
    for (uint32_t i = 0; i < code_size(c); i++) { //loop through the code c
        if (((c->bits[i / 8] & (1 << (i % 8))) >> (i % 8)) == 1) { //get bit of c at i
            bufcode[index / 8] |= (1 << (index % 8)); //set_bit(buffer,index)
        } else {
            bufcode[index / 8] &= ~(1 << (index % 8)); //clr bit
        }
        index += 1; //increment the index
        if (index == 8 * BLOCK) { //if buffer is full write it out
            write_bytes(outfile, bufcode, BLOCK); //write out buffer
            index = 0; //restart index
        }
    }
    return;
}

void flush_codes(int outfile) {
    if (index > 0) { //check if there is leftover bytes
        int nbytes = index % 8 == 0 ? index / 8 : (index / 8) + 1; //remaining bytes in the buffer
        write_bytes(outfile, bufcode, nbytes); //flush out the leftover bytes
    }
    return;
}
