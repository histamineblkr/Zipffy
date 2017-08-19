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
#include <ctype.h>
#include <string.h>

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
 * Using a file pointer, get the first word and then pass a copy of the word
 * back
 *
 * @returns: a string
 *
 */
char* getWord(char* line, int* idx)
{
    char word[50] = "word123";
    int wordIdx = 0;

    // Sanitize word
    for (int i = 0; i < 50; i++)
    {
        word[i] = '\0';
    }

    for (; line[*idx] != '\n'; *idx = (*idx + 1))
    {
        if (isalpha(line[*idx]) || (line[*idx] == '-'))
        {
            //fprintf(stdout, "%c", line[*idx]);
            word[wordIdx++] = tolower(line[*idx]);
            //fprintf(stdout, "line[%d]: %c\n", *idx, line[*idx]);
        }
        else if (ispunct(line[*idx]))
        {
            // skip punct, maybe return word
            //*idx = (*idx + 1);
        }
        else if (isspace(line[*idx]))
        {
            // Set word terminator
            /*word[++wordIdx] = '\0';
            fprintf(stdout, "%s:\t\t", word);
            unsigned long hash_output = hash(word);
            fprintf(stdout, "%d\n", hash_output);*/
            *idx += 1;
            return strdup(word);
        }
    }

    *idx += 1;
    return strdup(word);
}

/*
 * Process file. Tokenize each line and process each word.
 *
 * @returns:
 *
 * TODO: Process file.
 */
void processFile(FILE* textFp)
{
    // Variables to hold:
    // a line for text
    // a word once it is parsed
    // and an index to keep track of the line
    char line[256];
    char* word = (char*) malloc(sizeof(char));
    int* lineIdx = (int*) malloc(sizeof(int));

    // Set the line index to keep track of the line
    *lineIdx = 0;

    while (fgets(line, sizeof(line), textFp))
    {
        // Get line character Count
        int m;
        int charcount;

        charcount = 0;
        for(m = 0; line[m]; m++)
        {
            if(line[m] != '\n')
            {
                charcount ++;
            }
        }

        fprintf(stdout, "line: %s", line);
        fprintf(stdout, "charcount: %d\n", charcount);
        fprintf(stdout, "lineIdx: %d\n", *lineIdx);

        // Get word
        while (*lineIdx < (charcount - 1))
        //int c = 0;
        //while (c < 3)
        {
            word = getWord(line, lineIdx);
            fprintf(stdout, "After getting word: %s\n", word);
            unsigned long hash_output = hash(word);
            fprintf(stdout, "hash of word: %d\n", hash_output);
            fprintf(stdout, "lineIdx: %d\n", *lineIdx);
            //c++;
        }

        fprintf(stdout, "\n=====\n\n");

        // Reset line index to 0 for new line
        *lineIdx = 0;
    }

    // Free pointers
    free(lineIdx);
    free(word);
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


    /************* Process File *******************/
    fprintf(stdout, "\n");
    fprintf(stdout, "=====================================\n");

    processFile(fp);

    fprintf(stdout, "=====================================\n");
    fprintf(stdout, "\n");

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
