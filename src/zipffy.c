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
#include <ctype.h>
#include <string.h>
#include <unistd.h>

// Self written includes for project
#include "wordcount.h"
#include "hash.h"

// Global debug
bool DEBUG = false;

// Constants
static const int WORD_SIZE = 50;
static const int LINE_SIZE = 1024;
static const int MSG_SIZE = 1024;
static const int HASHTABLE_SIZE = 1300051;

/*
 * A structure to hold each work, its hash key, and count.
 *
 */
/*struct HashWords
{
    char value[50];
    int key;
    int count;
};*/

/*
 * Quick and dirty check for file passed in ending in .txt
 * Does not actually guarantee that the file is text.
 *
 * @returns: boolean true if file ends in ".txt", false if not
 *
 * TODO: Better text file checking.
 */
static bool isTextFile(char* fname)
{
    regex_t regex;
    int reti;
    char msgbuf[MSG_SIZE];
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
 * Count all characters in a line and strip the newline or linefeed (\r, \n))
 * and replace with \0
 *
 * @returns: an integer   the character count
 *
 */
static int countCharsAndStripNewline(char* line)
{
    int charCount = 0;

    while ((line[charCount] != '\n' && line[charCount] != '\r'))
    {
        charCount++;
    }

    line[charCount] = '\0';

    return charCount;
}

/*
 * Count all words in a line
 *
 * @returns: an integer   the word count
 *
 */
static int countWordsInLine(char* line)
{
    int wordCount = 1;

    if (   strcmp(" ",  line)
        || strcmp("\0", line)
        || strcmp("\n", line)
        || strcmp("\r", line) )
    {
        return wordCount = 0;
    }

    for (int m = 0; line[m] != '\0'; m++)
    {
        if ((line[m] == ' ' && m != 0) && (line[m - 1] != ' '))
        {
            wordCount++;
        }
    }

    return wordCount;
}

/*
 * Clean word and or initialize
 *
 */
static void sanitizeWord(char* word)
{
    for (int i = 0; i < WORD_SIZE; i++)
    {
        word[i] = '\0';
    }
}

/*
 * Using a file pointer, get the first word and then pass a copy of the word
 * back
 *
 * @returns: a boolean of true when word is built
 *
 * TODO: Find issue in here causing memory error
 */
static bool getWord(char* line, int* idx, char* word)
{
    int wordIdx = 0;

    // Build word character by character
    for ( ; (line[*idx] != '\0' && wordIdx < WORD_SIZE); *idx = (*idx + 1))
    {
        // Hit word size limit
        if (wordIdx == (WORD_SIZE - 1))
        {
            word[wordIdx] = '\0';
            *idx += 1;
            return true;
        }

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
static void processFile(FILE* textFp, hashtable_t* hashtable)
{
    char line[LINE_SIZE];
    //unsigned char word[WORD_SIZE];
    char* word = malloc(sizeof(char) * 50);
    int* lineIdx = (int*) malloc(sizeof(int));
    int lineCount = 1;

    // Intialize memory for char arrays
    //memset(word, '\0', (WORD_SIZE * sizeof(unsigned char)));
    strcpy(word, "");

    // Set the line index to keep track of the line
    *lineIdx = 0;

    while (fgets(line, sizeof(line), textFp) != NULL)
    {
        if (strcmp("",   line) == 0) { continue; }
        if (strcmp(" ",  line) == 0) { continue; }
        if (strcmp("\0", line) == 0) { continue; }
        if (strcmp("\n", line) == 0) { continue; }
        if (strcmp("\r", line) == 0) { continue; }

        // Get character count of the line
        int charcount = countCharsAndStripNewline(line);

        // Get word count of the line
        int wordCount = countWordsInLine(line);

        if (DEBUG == true)
        {
            fprintf(stdout, "line %d:\n", lineCount);
            fprintf(stdout, "  words in line: %d\n", wordCount);
            fprintf(stdout, "  charcount: %d\n", charcount);
            fprintf(stdout, "  lineIdx: %d\n", *lineIdx);
            fprintf(stdout, "  value: \"%s\"\n\n", line);
        }

        // Get words from line and hash the word
        unsigned long key = 0;
        while (*lineIdx < (charcount - 1))
        {
            sanitizeWord(word);
            getWord(line, lineIdx, word);
            setItem(hashtable, word);
            key = hash(hashtable->size, word);

            //key = hash(word);
            //ht_set(hashtable, key, word);

            if (DEBUG == true)
            {
                fprintf(stdout, "key: %lu\n", key);
                fprintf(stdout, "  value: %s\n", getValue(hashtable, key));
                fprintf(stdout, "  count: %3d\n", getCount(hashtable, key));
                fprintf(stdout, "  lineIdx: %2d\n", *lineIdx);
            }
        } // End while for word

        // Reset line index to 0 for new line
        *lineIdx = 0;
        lineCount++;

        if (DEBUG == true) { fprintf(stdout, "\n========\n\n"); }
    } // End while for line

    // Free pointers
    free(word);
    free(lineIdx);

    if (DEBUG == true) { if (feof(textFp)) { fprintf(stderr, "Reached FEOF.\n"); } }
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

    // Get rough estimate of total words in text file (many could be duplicate)
    // This will give a good approximation of the hash table size
    int totalWords = 0;
    totalWords = countWords(fp);

    // Rewind pointer so we can process file
    rewind(fp);

    // Create hashtable
    hashtable_t* hashtable = createTable(HASHTABLE_SIZE);

    // Process file
    processFile(fp, hashtable);

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

    fprintf(stdout, "Total Number of Words: %d\n", totalWords);

    // Display hashtable
    printf("\nPrinted Hash:\n");
    for (int i = 0; i < HASHTABLE_SIZE; i++)
    {
        if (hashtable->entry[i] != NULL)
        {
            printf("  value: %25s,\t", hashtable->entry[i]->value);
            printf("hash: %10lu,\t", hashtable->entry[i]->key);
            printf("count: %d\n", hashtable->entry[i]->count);
        }
    }

    // Visualize with +'s
    /*for (int i = 0; i < HASHTABLE_SIZE; i++)
    {
        if (hashtable->entry[i] != NULL)
        {
            printf("%20s:\t", hashtable->entry[i]->value);
            for (int j = 0; j < hashtable->entry[i]->count; j++)
              printf("+");
            printf("\n");
        }
    }*/


  //++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
  //        FREE ALL MEMORY THAT HASN'T BEEN FREED YET
  //++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

    deleteTable(hashtable);
    free(hashtable);

    exit(0);
}
