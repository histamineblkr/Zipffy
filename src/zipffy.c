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

// Includes for program to run
#include <stdio.h>
#include <stdlib.h>
#include <regex.h>
#include <stdbool.h>
#include <sys/types.h>
#include <regex.h>
#include <ctype.h>
#include <string.h>
#include <unistd.h>

// Global debug
bool DEBUG = false;

/*
 * A structure to hold each work, its hash key, and count.
 *
 */
struct HashWords
{
    char word[50];
    int key;
    int count;
};


/* A djb2 hash function sourced online.
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
        fprintf(stderr, "Could not compile regex.\n");
        exit(1);
    }

    // Execute regular expression
    reti = regexec(&regex, filename, 0, NULL, 0);
    if (!reti)
    {
        if (DEBUG == true) { fprintf(stdout, "File appears to be a text file.\n"); }
        regfree(&regex);
        return true;
    }
    else if (reti == REG_NOMATCH)
    {
        fprintf(stderr, "File does not appear to be a text file.\n");
        regfree(&regex);
        return false;
    }
    else
    {
        regerror(reti, &regex, msgbuf, sizeof(msgbuf));
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
 * @returns: a boolean of true when word is built
 *
 */
//char* getWord(char* line, int* idx)
bool getWord(char* line, int* idx, char* word)
{
    int wordIdx = 0;

    // Build word character by character
    for (; line[*idx] != '\0'; *idx = (*idx + 1))
    {
        if (isalpha(line[*idx]) || (line[*idx] == '-'))
        {
            word[wordIdx++] = tolower(line[*idx]);
        }
        else if (isspace(line[*idx]))
        {
            *idx += 1;
            return true;
        }
    }

    return true;
}

/*
 * Process file. Tokenize each line and process each word.
 *
 * TODO: Process file.
 */
void processFile(FILE* textFp, int* numOfWords)
{
    // Variables to hold:
    //   a line for text
    //   a word once it is parsed
    //   an index to keep track of the line
    char line[256];
    unsigned char* word = (unsigned char*) malloc(sizeof(char) * 50);
    int* lineIdx = (int*) malloc(sizeof(int));
    int lineCount = 1;

    // Set the line index to keep track of the line
    *lineIdx = 0;

    while (fgets(line, sizeof(line), textFp))
    {
        // Get line character Count
        int m = 0;
        int charcount = 0;
        int wordCount = 1;

        for(m = 0; line[m]; m++)
        {
            // By counting spaces, you can get a rough estimate of how many words
            // are in each line. (totalSpaces + 1)
            if ((line[m] == ' ') && (line[m-1] != ' '))
            {
                 wordCount++;
            }

            if(line[m] != '\n')
            {
                charcount++;
            }
            else
            {
                line[m] = '\0';
            }
        }

        // Update word total
        *numOfWords += wordCount;

        // Create hash structure
        struct HashWords HashWord;

        if (DEBUG == true)
        {
            fprintf(stdout, "line %d:\n", lineCount);
            fprintf(stdout, "  words in line: %d\n", wordCount);
            fprintf(stdout, "  total words: %d\n", *numOfWords);
            fprintf(stdout, "  charcount: %d\n", charcount);
            fprintf(stdout, "  lineIdx: %d\n", *lineIdx);
            fprintf(stdout, "  value: \"%s\"\n\n", line);
        }

        // Get word
        while (*lineIdx < (charcount - 1))
        {
            // Sanitize word
            for (int i = 0; i < 50; i++)
            {
                word[i] = '\0';
            }

            getWord(line, lineIdx, word);
            unsigned long hash_output = hash(word);

            strcpy(HashWord.word, word);
            HashWord.key = hash_output;

            if (DEBUG == true)
            {
                fprintf(stdout, "getWord(): %8s,\t", HashWord.word);
                fprintf(stdout, "hash(): %10d,\t", HashWord.key);
                fprintf(stdout, "lineIdx: %2d\n", *lineIdx);
            }
        }

        if (DEBUG == true) { fprintf(stdout, "\n========\n\n"); }

        // Reset line index to 0 for new line
        *lineIdx = 0;
        lineCount++;
    }

    // Free pointers
    free(lineIdx);
    free(word);
}


// Main
int main (int argc, char* argv[])
{
  //++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
  //        VERIFY COMMAND LINE ARGUMENTS NECESSARY FOR PROGRAM
  //++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

    // User did not pass in any argument
    if (argc == 1)
    {
        fprintf(stderr, "usage: zipffy afile.txt\n");
        exit(-1);
    }

    // User passed text file hopefully,
    if (argc == 2)
    {
        if (!isTextFile(argv[1]))
        {
            fprintf(stderr, "Please pass in a text file.\n");
            exit(-1);
        }
    }

    // Grab text file, possibly turn on debug, and ignore other arguments
    if (argc >= 3)
    {
        // For debug purposes
        if (strcmp("-d", argv[2]) == 0)
        {
            DEBUG = true;
            fprintf(stdout, "+++++++++++++++++++++++++++++++++++++++\n");
            fprintf(stdout, "+            [DEBUGGING ON]           +\n");
            fprintf(stdout, "+++++++++++++++++++++++++++++++++++++++\n\n");
        }

        if (!isTextFile(argv[1]))
        {
            fprintf(stderr, "Please pass in a text file.\n");
            exit(-1);
        }
    }


  //++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
  //        PROCESS PASSED IN TEXT FILE
  //++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

    // Open file for reading
    FILE* fp = fopen(argv[1], "r+");

    // If fp is NULL, file does not exist
    if (fp == 0)
    {
        fprintf(stderr, "File does not exist.\n");
        exit(1);
    }

    if (DEBUG == true) { fprintf(stdout, "File exists.\n"); }

    if (DEBUG == true)
    {
        fprintf(stdout, "\n");
        fprintf(stdout, "================================================================================\n");
    }

    // Process text file for zipfs law and hashing
    int* wordTotal = (int*) malloc(sizeof(int));
    *wordTotal = 0;
    processFile(fp, wordTotal);

    // Close file pointer
    if (fclose(fp) != 0)
    {
        fprintf(stderr, "File did not close.\n");
    }

    if (DEBUG == true) { fprintf(stdout, "File closed.\n"); }

    if (DEBUG == true)
    {
        fprintf(stdout, "================================================================================\n");

        fprintf(stdout, "\n");
    }


  //++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
  //        DISPLAY ZIPFS LAW OUTPUT
  //++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

    fprintf(stdout, "+++++++++++++++++++++++++++++++++++++++\n");
    fprintf(stdout, "+           Zipfs Output Below        +\n");
    fprintf(stdout, "+++++++++++++++++++++++++++++++++++++++\n\n");

    fprintf(stdout, "Total Number of Words: %d\n", *wordTotal);


  //++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
  //        FREE ALL MEMORY THAT HASN'T BEEN FREED YET
  //++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

    free(wordTotal);

    exit(0);
}
