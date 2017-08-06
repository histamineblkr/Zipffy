/*
 * License: GPLv3
 *
 * File: zipffy.c
 *
 * Description: This file is the main program file that will ultimately display
 *              Zipf's Law when given any text file. It will handle the program
 *              logic and displaying the results.
 *
 * Author:  Brandon Authier (hblkr)
 * Date:    6 Aug 2017
 * Version: 1.0
 *
 */
#include <stdio.h>
#include <stdlib.h>
#include <regex.h>

// Hash function
unsigned long hash(unsigned char *str)
{
    unsigned long hash = 5381;
    int c;

    while (c = *str++)
    hash = ((hash << 5) + hash) + c; /* hash * 33 + c */

    return hash;
}

// Main
int main(int argc, char* argv[])
{

    // User did not pass in any argument
    if (argc == 1)
    {
        fprintf(stderr, "usage: zipffy afile.txt\n");
        return -1;
    }

    // User passed in at least one argument, take first argument and ignore rest
    if (argc >= 2)
    {
      
    }

    /******* Begin Zipfs Law Computation ********/

    // Hash
    unsigned long hash_output = hash(argv[1]);

    // Display output
    fprintf(stdout, "argument 1: %s\n", argv[1]);
    fprintf(stdout, "hash: %d\n", hash_output);

    return 0;
}
