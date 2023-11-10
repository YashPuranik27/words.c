#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <ctype.h>

typedef struct {
    char *word;
    int count;
} WordCount;

WordCount *words = NULL;
int num_words = 0;

int is_valid_char(char c) {
    return isalpha((unsigned char)c) || c == '\'' || c == '-';
}

// This should determine if the character is a word separator
int is_separator(char c) {
    return !isalpha((unsigned char)c) && c != '\'' && c != '-';
}

// This should check if a character is a valid hyphen within a word
int is_valid_hyphen(char prev, char next) {
    return isalpha((unsigned char)prev) && isalpha((unsigned char)next);
}

// This is a helper function to check if a character is a letter, an apostrophe, or a valid hyphen
int is_word_char(char prev, char c, char next) {
    if (isalpha((unsigned char)c)) return 1;
    if (c == '\'') return 1;
    if (c == '-' && isalpha((unsigned char)prev) && isalpha((unsigned char)next)) return 1;
    return 0;
}

void insert_word(char *word) {
    for (int i = 0; i < num_words; i++) {
        if (strcmp(words[i].word, word) == 0) {
            words[i].count++;
            return;
        }
    }
    words = realloc(words, sizeof(WordCount) * (num_words + 1));
    words[num_words].word = strdup(word);
    words[num_words].count = 1;
    num_words++;
}

int cmp(const void *a, const void *b) {
    WordCount *wa = (WordCount *)a;
    WordCount *wb = (WordCount *)b;
    if (wb->count - wa->count != 0)
        return wb->count - wa->count;
    return strcmp(wa->word, wb->word);
}

// This is a function to process a file and count the words according to the given rules
void process_file(char *filename) {
    int fd = open(filename, O_RDONLY);
    if (fd < 0) return;

    struct stat st;
    if (fstat(fd, &st) != 0) {
        close(fd);
        return;
    }

    char *buffer = (char *)malloc(st.st_size + 1);
    ssize_t bytes_read = read(fd, buffer, st.st_size);
    if (bytes_read != st.st_size) {
        close(fd);
        free(buffer);
        return;
    }
    buffer[bytes_read] = '\0';
    close(fd);

    char *p = buffer;
    char *word_start = NULL;
while (*p) {
    // If it's an alpha character, or an apostrophe followed by an alpha, potentially part of a word
    if (isalpha((unsigned char)*p) || (*p == '\'' && isalpha((unsigned char)*(p + 1)))) {
        if (!word_start) {
            word_start = p; // Start of a new word
        }
    } else if (*p == '-' && word_start) {
        // Check if it's a valid hyphen (surrounded by letters)
        if (is_valid_hyphen(*(p - 1), *(p + 1))) {
            // It's a valid hyphen, continue the word
        } else {
            // It's not a valid hyphen, end the current word
            *p = '\0';
            insert_word(word_start);
            word_start = NULL;
        }
    } else {
        // If we've started a word, it's now time to end it
        if (word_start) {
            *p = '\0';
            insert_word(word_start);
            word_start = NULL;
        }
    }
    p++;
}

    // This handles the last word in the buffer, if there is one
    if (word_start) {
        insert_word(word_start);
    }

    free(buffer);
}


void process_directory(char *dirname) {
    DIR *dir = opendir(dirname);
    if (!dir) return;

    struct dirent *entry;
    struct stat entry_stat;
    char path[1024];

    while ((entry = readdir(dir)) != NULL) {
        snprintf(path, sizeof(path), "%s/%s", dirname, entry->d_name);

        if (stat(path, &entry_stat) == 0 && S_ISDIR(entry_stat.st_mode) && strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0) {
            process_directory(path);
        } else if (strstr(entry->d_name, ".txt")) {
            process_file(path);
        }
    }
    closedir(dir);
}

int main(int argc, char *argv[]) {
    for (int i = 1; i < argc; i++) {
        struct stat st;
        stat(argv[i], &st);
        if (S_ISDIR(st.st_mode)) {
            process_directory(argv[i]);
        } else {
            process_file(argv[i]);
        }
    }

    qsort(words, num_words, sizeof(WordCount), cmp);

    for (int i = 0; i < num_words; i++) {
        printf("%s %d\n", words[i].word, words[i].count);
        free(words[i].word);
    }
    free(words);

    return 0;
}
