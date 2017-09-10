# Zipffy #
Zipffy will display Zipf's Law when given any text file.

Zipf's law states that given a large sample of words used, the frequency of any
word is inversely proportional to its rank in the frequency table. So word
number N has a frequency of 1/N.

Thus the most frequent word will occur about twice as often as the second most
frequent word, three times as often as the third most frequent word, etc.

## Installation ##
Program doesn't need installation.

### Make ###
Compile yourself and run from the **dist/** folder:

    cd Zipffy
    make
    dist/zipffy file.txt

Alternatively you can run in a debug mode:

    dist/zipffy file.txt -d

## Features ##
- Reads in a text file
- Displays Zipf's Law visually

## Considerations ##
- This project decides to use Mersenne Primes (5 through 9) which is:
    - 8191 to 2305843009213693951, any text with a larger word count than that seems a bit too much. Also, that takes into consideration the LONG_MAX for a long int set in the limits.h.
- Uses the djb2 hash [here](http://www.cse.yorku.ca/~oz/hash.html)
- Assumes English language and longest word is: "pneumonoultramicroscopicsilicovolcanoconiosis" (46 Characters)
- How to handle contractions: i.e. "it's"?
    - Count as a single word "it's" or count as two: "it" and "is"?
    - Program chooses the latter and discards the apostrophe. Yes I realize that "its" and "it's" are two different words, one a possesive form of "it" and the other a contraction of "it" and "is", but for now I am choosing to treat those as the same and will later implement a way of distinguishing between the two.

## Contribute ##
- Source Code: github.com/histamineblkr/Zipffy
- Issue Tracker: github.com/histamineblkr/Zipffy/issues

## Support ##
If you are having issues, please let me know.
We have a mailing list located at: zipffy@gmail.com

## License ##
The project is licensed under the GPLV3 license.
