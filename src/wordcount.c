#include "wordcount.h"

int countWords(FILE* textFp)
{
    char line[4096];
    int wordCount = 0;

    while (fgets(line, sizeof(line), textFp))
    {
        if (strcmp("",   line) == 0) { continue; }
        if (strcmp(" ",  line) == 0) { continue; }
        if (strcmp("\0", line) == 0) { continue; }
        if (strcmp("\n", line) == 0) { continue; }
        if (strcmp("\r", line) == 0) { continue; }

        /* Counting spaces gives a rough estimate of how many words */
        // Assume first space is first word
        int subtotal = 1;

        for (int m = 0; line[m] != '\n'; m++)
        {
            if ((line[m] == ' ' && m != 0) && (line[m - 1] != ' '))
            {
                subtotal++;
            }
        }

        wordCount += subtotal;
    }

    return wordCount;
}
