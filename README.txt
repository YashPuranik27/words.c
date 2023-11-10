Yash Puranik yap13
Zixin Zheng zz585


*Program description:*

This is a program written in C that can be used to count the number of words in text files. It can work with both single files and folders that hold text files. The main features of the program are broken down into several functions, each of which counts words in a different way.

The WordCount function is set up to keep track of words and how often they appear. The global words pointer keeps a list of WordCount instances up to date, and num_words keeps track of how many unique words were found.

We use the methods is_valid_char, is_separator, and is_valid_hyphen to find out what kind of character it is (a part of a word, a separator, or a valid hyphen within a word). These are used by the built-in helper function is_word_char to check if a character is part of a word.

When the insert_word method is called, it either adds a new WordCount instance to the array or raises the count of an existing WordCount instance.

qsort sorts the WordCount array first by frequency (in decreasing order) and then by letter. The cmp function is a comparison that does this.

The process_file function reads a file into a buffer, turns the buffer into words using the rules set by the helper functions, and then adds these words to the words array.

In a loop, the process_directory function reads each file in a directory that ends in ".txt" and then passes the work to the process_file function.

Lastly, the main method takes command-line arguments, which let the user choose which directories or files to process. Once the work is done, the words are put in order and their counts are written to the standard output. Any memory that was used is then released.

Some notable changes/decisions that went into this code:

We used a dynamic memory allocation for the words array, which lets the computer deal with any number of unique words.

we used the stat system call to find out if a path is a directory or a file.

The program is made to handle directories and up to 2 subdirectories in a loop, which makes it useful for analyzing large groups of text files.

The code smartly handles word boundaries, which lets words with hyphens and apostrophes be found properly.

The computer frees up dynamically allocated memory at the end of directory traversal. This stops memory leaks.
