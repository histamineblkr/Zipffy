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
#include <stdbool.h>
#include <sys/types.h>
#include <regex.h>

/* A simple hash function sourced online.
 */
unsigned long hash(unsigned char *str)
{
    unsigned long hash = 5381;
    int c;

    while (c = *str++)
    hash = ((hash << 5) + hash) + c; /* hash * 33 + c */

    return hash;
}

/*
 * Quick and dirty check for file passed in ending in .txt
 * Does not actually guarantee that the file is text.
 *
 * @returns: boolean true if file ends in ".txt", false if not
 *
 * TODO: Better text file checking.
 */
bool isTextFile(char* fname)
{
    regex_t regex;
    int reti;
    char msgbuf[100];
    char* filename = fname;
    char* filenameRegex = ".txt$";

    // Compile regular expression
    reti = regcomp(&regex, filenameRegex, 0);

    if (reti)
    {
        fprintf(stderr, "Could not compile regex\n");
        exit(1);
    }

    // Execute regular expression
    reti = regexec(&regex, filename, 0, NULL, 0);
    if (!reti)
    {
        fprintf(stdout, "File appears to be a text file.\n");
        // Free compiled regular expression
        regfree(&regex);
        return true;
    }
    else if (reti == REG_NOMATCH)
    {
        fprintf(stderr, "File does not appear to be a text file.\n");
        // Free compiled regular expression
        regfree(&regex);
        return false;
    }
    else
    {
        regerror(reti, &regex, msgbuf, sizeof(msgbuf));
        // Free compiled regular expression
        regfree(&regex);
        fprintf(stderr, "A Regex match error occured: %s\n", msgbuf);
        fprintf(stderr, "Exiting.\n");
        exit(1);
    }
}

/*
 * Process file. Tokenize each line and process each word.
 *
 * @returns:
 *
 * TODO: Process file.
 */
void zipfTextFile(FILE* textFp)
{

}


// Main
int main(int argc, char* argv[])
{

    // User did not pass in any argument
    if (argc == 1)
    {
        fprintf(stderr, "usage: zipffy afile.txt\n");
        exit(-1);
    }

    // User passed in at least one argument, take first argument and ignore rest
    if (argc >= 2)
    {
        if (!isTextFile(argv[1]))
        {
            fprintf(stderr, "Please pass in a text file.\n");
            exit(-1);
        }
    }

    // Open file for reading
    FILE* fp = fopen(argv[1], "r+");

    // If fp is NULL, file does not exist
    if (fp == 0)
    {
        fprintf(stderr, "File does not exist.\n");
        exit(1);
    }
    fprintf(stdout, "File exists.\n");

    // Close file pointer
    if (fclose(fp) != 0)
    {
        fprintf(stderr, "File did not close.\n");
    }
    fprintf(stdout, "File closed.\n");


    /******* Begin Zipfs Law Computation ********/

    // Hash !!!!!! Example using passed in string file !!!!!!!
    unsigned long hash_output = hash(argv[1]);

    // Display output
    fprintf(stdout, "argument 1: %s\n", argv[1]);
    fprintf(stdout, "hash of argv[1]: %d\n", hash_output);

    return 0;
}
