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

int main(int argc, char **argv) {
    int32_t opt = 0;
    int input = STDIN_FILENO;
    int output = STDOUT_FILENO;
    bool verbose = false;

    while ((opt = getopt(argc, argv, OPTIONS)) != -1) { //program argument parser.
        switch (opt) {
        case 'h':
            //print message
            fprintf(stderr,
                "SYNOPSIS\n"
                "  A Huffman decoder.\n"
                "  Decompresses a file using the Huffman coding algorithm.\n"
                "\n"
                "USAGE\n"
                "  %s [-h] [-i infile] [-o outfile]\n"
                "\n"
                "OPTIONS\n"
                "  -h             Program usage and help.\n"
                "  -v             Print compression statistics.\n"
                "  -i infile      Input file to decompress.\n"
                "  -o outfile     Output of decompressed data.\n",
                argv[0]);
            exit(1);
            break;
        case 'i': input = open(optarg, O_RDONLY); break;
        case 'o': output = open(optarg, O_WRONLY | O_CREAT | O_TRUNC, 0600); break;
        case 'v': verbose = true; break;
        }
    }

    //read header of file
    struct Header header1;
    read_bytes(input, (uint8_t *) &header1, sizeof(Header)); //print header on std

    if (header1.magic != MAGIC) { //check if the file is valid and my program is able to decode it
        fprintf(stderr, "Error: unable to read header.\n");
        exit(1);
    }

    //Set output persmission same as input permission
    fchmod(output, header1.permissions);

    //rebuild tree from dump
    uint8_t dump_tree
        [header1.tree_size]; //creates a buffer of dump tree with the tree size I got from header
    read_bytes(input, dump_tree, header1.tree_size); //reads in bytes in order to occupy dump tree
    Node *tree = rebuild_tree(
        header1.tree_size, dump_tree); //rebuilds orginal tree made in encode to use later

    ////dump file
    Node *curr = tree; //creates temp tree in order to decode message later
    uint8_t buf; //buffer to contain the bit read from read_bit
    uint64_t numdecoded = 0; //keeps track of numbers of chars decoded in order to end once the file
    //has been read fully
    while (
        read_bit(input, &buf) && curr != NULL) { //loop to read the rest of the bits after tree bit
        if (buf == 0) { //if bit is 0 head to left node
            curr = curr->left;
        } else { //if bit is 1 head to the right node
            curr = curr->right;
        }
        if (curr != NULL
            && (curr->right == NULL && curr->left == NULL)) { //if current location is a leaf
            write_bytes(output, &curr->symbol, 1); //write the current leaf symbol out
            numdecoded += 1; //increase the number of numdecoded to test later
            curr = tree; //return to curr to tree to read next code
        }
        if (numdecoded == header1.file_size) { //if at the end of file end loop
            break; //exit while loop
        }
    }

    if (verbose) { //if verbose is enable prints statsitics about the fil
        printf("Compressed file size: %lu bytes\n", bytes_read);
        printf("Decompressed file size: %lu bytes\n", bytes_written);
        printf(
            "Space saving: %.2f%%\n", (double) 100 * (1 - ((double) bytes_read / bytes_written)));
    }
    ////close program
    delete_tree(&tree); //deletes tree
    close(output); //closes the opened output file
    close(input); //closes the opened input file
    return 0; //exit sucessfully
}
