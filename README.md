# Zipffy #
Zipffy will display Zipf's Law when given any text file.

Zipf's law states that given a large sample of words used, the frequency of any
word is inversely proportional to its rank in the frequency table. So word
number N has a frequency of 1/N.

Thus the most frequent word will occur about twice as often as the second most
frequent word, three times as often as the third most frequent word, etc.

## Make ##
Doesn't need installation. Compile yourself and run from the **dist/** folder:

    make zipffy
    dist/zippfy afile.txt

## Features ##
- Reads in any text file
- Displays Zipf's Law visually

## Considerations ##
- Assumes English language and longest word is: "pneumonoultramicroscopicsilicovolcanoconiosis" (46 Characters)
- How to handle contractions: i.e. "it's"?
    - Count as single word "it's" or count as two: "it" and "is"?

## Contribute ##
- Issue Tracker: github.com/histamineblkr/Zipffy/issues
- Source Code: github.com/histamineblkr/Zipffy

## Support ##
If you are having issues, please let me know.
We have a mailing list located at: zipffy@gmail.com

## License ##
The project is licensed under the GPLV3 license.
