#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>

typedef struct {
    char *word;
    int count;
} WordCount;

WordCount *words = NULL;
int num_words = 0;

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

void process_file(char *filename) {
    int fd = open(filename, O_RDONLY);
    if (fd < 0) return;

    struct stat st;
    fstat(fd, &st);
    char *buffer = malloc(st.st_size + 1);
    read(fd, buffer, st.st_size);
    buffer[st.st_size] = '\0';
    close(fd);

    char *token = strtok(buffer, " \n\r\t");
    while (token) {
        insert_word(token);
        token = strtok(NULL, " \n\r\t");
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
