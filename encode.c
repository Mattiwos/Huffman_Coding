#include "code.h"
#include "defines.h"
#include "header.h"
#include "huffman.h"
#include "io.h"
#include "node.h"

#include <fcntl.h>
#include <inttypes.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h> //need for strdup
#include <sys/stat.h> //used to set permision
#include <sys/types.h>
#include <unistd.h> //for getop()

#define OPTIONS "hi:o:v" //options for input

void tree_dump(Node *root, int outfile) {
    if (root) {
        if (root->right == NULL && root->left == NULL) { //at leaf node

            uint8_t buf[2] = { (uint8_t) 'L', root->symbol };
            write_bytes(outfile, buf, 2); //leaf reach print symbol and L

        } else { // if (root->right != NULL && root->left != NULL) {

            tree_dump(root->left, outfile); //explore left node
            tree_dump(root->right, outfile); //explore right node

            uint8_t buf = (uint8_t) 'I'; // { (uint8_t) 'I' }; // I
            write_bytes(outfile, &buf, 1); //go back a node and print I
        }
    }
    return;
}

int main(int argc, char **argv) {
    int32_t opt = 0;
    int input = STDIN_FILENO;
    int output = STDOUT_FILENO;
    bool verbose = false;
    int uniquesymb = 0;
    while ((opt = getopt(argc, argv, OPTIONS)) != -1) { //program argument parser.
        switch (opt) {
        case 'h':
            //print message
            fprintf(stderr,
                "SYNOPSIS\n"
                "  A Huffman encoder.\n"
                "  Compresses a file using the Huffman coding algorithm.\n"
                "\n"
                "USAGE\n"
                "  %s [-h] [-i infile] [-o outfile]\n"
                "\n"
                "OPTIONS\n"
                "  -h             Program usage and help.\n"
                "  -v             Print compression statistics.\n"
                "  -i infile      Input file to compress.\n"
                "  -o outfile     Output of compressed data.\n",
                argv[0]); //update later1
            exit(1);
            break;
        case 'i': input = open(optarg, O_RDONLY); break;
        case 'o': output = open(optarg, O_WRONLY | O_CREAT | O_TRUNC, 0600); break;
        case 'v': verbose = true; break;
        }
    }

    struct stat statbuff;
    fstat(input, &statbuff);
    uint64_t file_size = statbuff.st_size; //get input file size
    uint16_t perm = statbuff.st_mode; //get input permission

    //builds histogram
    uint64_t hist[ALPHABET] = { 0 }; //creates a histogram and inilizes it to 0
    hist[0] += 1; //adds 1 to 0
    hist[ALPHABET - 1] += 1; //add 1 to 255 in order to have a min of 2 chars in historgram
    uint8_t buf; //buffer for bytes of char for histogram
    while (read_bytes(input, &buf, 1) != 0) { //reads the entire file and counts frequency
        hist[buf] += 1;
    }
    //counts uniqueue symbols
    for (uint64_t i = 0; i < ALPHABET; i++) {
        if (hist[i] >= 1) {
            uniquesymb += 1;
        }
    }

    //Builds Tree
    Node *tree = build_tree(hist);

    //Creates Code table
    Code *table = (Code *) calloc(ALPHABET, sizeof(Code));
    build_codes(tree, table);

    // Final Output to output file
    //
    fchmod(output, perm); //set output permission same as input

    //Header
    struct Header header1 = { 0, 0, 0, 0 }; //inilizes header
    header1.magic = MAGIC;
    header1.permissions = perm;
    header1.file_size = file_size;
    header1.tree_size = (uniquesymb * 3) - 1; //formula found on drive
    write_bytes(output, (uint8_t *) &header1, sizeof(Header)); //print header to output filen

    //tree dump
    tree_dump(tree, output);

    lseek(input, 0L, SEEK_SET); //start reading from the top

    bytes_read = 0; //reset bytes read because its going to reread it again

    //emit code
    while (read_bytes(input, &buf, 1) != 0) {
        write_code(output, &table[buf]);
    }
    flush_codes(output);

    if (verbose) { //if verbose is enabled print statistics about the encoding
        printf("Uncompressed file size: %lu bytes\n", bytes_read);
        //divided bytes read by 2 since infile read twice
        printf("Compressed file size: %lu bytes\n", bytes_written);
        printf(
            "Space saving: %.2f%%\n", (double) 100 * (1 - ((double) bytes_written / bytes_read)));
    }

    //close program
    free(table);
    delete_tree(&tree); //delete tree
    close(output); //close output file
    close(input); //close input file
    return 0; //exit successfully
}
