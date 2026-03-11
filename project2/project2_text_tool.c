/*
 * Project 2: Adaptive Text Intelligence Tool
 * Student Email: d.akol@alustudent.com
 * Custom Analysis: Average Word Length
 * Source paragraph: Opening lines of "The Alchemist" by Paulo Coelho
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

/* Structure for storing word statistics */
struct wordStat {
    char name[40];
    int  count;
};

/* Returns 1 if the character is a word boundary */
static int is_delim(char c) {
    return (c == ' ' || c == '\t' || c == '\n' || c == '\r' ||
            c == ',' || c == '.' || c == '!' || c == '?' ||
            c == ';' || c == ':' || c == '"' || c == '\'');
}

/* Converts a string to lowercase in place */
static void to_lower(char *s) {
    for (; *s; s++) *s = (char)tolower((unsigned char)*s);
}

/* Analysis function 1: count total words */
void count_words(char **words, int n) {
    printf("  Total word count : %d\n", n);
}

/* Analysis function 2: find the longest word */
void longest_word(char **words, int n) {
    if (n == 0) return;
    char *longest = words[0];
    for (int i = 1; i < n; i++)
        if (strlen(words[i]) > strlen(longest))   /* strlen demonstrated */
            longest = words[i];
    printf("  Longest word     : \"%s\" (%zu letters)\n", longest, strlen(longest));
}

/* Analysis function 3: find the most frequently occurring word */
void most_frequent(struct wordStat *stats, int unique) {
    if (unique == 0) return;
    struct wordStat *top = &stats[0];
    for (int i = 1; i < unique; i++)
        if (stats[i].count > top->count) top = &stats[i];
    printf("  Most frequent    : \"%s\" (%d times)\n", top->name, top->count);
}

/* Custom analysis: average word length */
void average_word_length(char **words, int n) {
    if (n == 0) return;
    long total = 0;
    for (int i = 0; i < n; i++) total += (long)strlen(words[i]);
    printf("  Avg word length  : %.2f characters\n", (double)total / n);
}

/*
 * Splits text into a word array using pointer traversal.
 * Returns the number of tokens found.
 * Caller is responsible for freeing each word and the array.
 */
int tokenise(char *text, char ***out_words) {
    int capacity = 64, n = 0;
    char **words = malloc(capacity * sizeof(char *));
    if (!words) { fprintf(stderr, "malloc failed\n"); exit(1); }

    char *p = text;   /* pointer traversal */
    while (*p) {
        /* skip delimiters */
        while (*p && is_delim(*p)) p++;
        if (!*p) break;

        /* mark start of token */
        char *start = p;
        while (*p && !is_delim(*p)) p++;

        /* copy token into new allocation */
        int len = (int)(p - start);
        char *token = malloc(len + 1);
        if (!token) { fprintf(stderr, "malloc failed\n"); exit(1); }
        strncpy(token, start, len);   /* strncpy demonstrated */
        token[len] = '\0';
        to_lower(token);

        if (n == capacity) {
            capacity *= 2;
            words = realloc(words, capacity * sizeof(char *));
        }
        words[n++] = token;
    }
    *out_words = words;
    return n;
}

/*
 * Builds a unique-word statistics array.
 * Uses strcmp to detect duplicates.
 * Returns the number of unique words found.
 */
int build_stats(char **words, int n, struct wordStat **out_stats) {
    struct wordStat *stats = malloc(n * sizeof(struct wordStat));
    if (!stats) { fprintf(stderr, "malloc failed\n"); exit(1); }
    int unique = 0;

    for (int i = 0; i < n; i++) {
        int found = 0;
        for (int j = 0; j < unique; j++) {
            if (strcmp(stats[j].name, words[i]) == 0) {   /* strcmp demonstrated */
                stats[j].count++;
                found = 1;
                break;
            }
        }
        if (!found) {
            strncpy(stats[unique].name, words[i], 39);
            stats[unique].name[39] = '\0';
            stats[unique].count    = 1;
            unique++;
        }
    }
    *out_stats = stats;
    return unique;
}

int main(void) {
    printf("========================================\n");
    printf("  Adaptive Text Intelligence Tool\n");
    printf("  Student ID: 1\n");
    printf("  Custom Analysis: Average Word Length\n");
    printf("========================================\n\n");

    /*
     * Sample paragraph source:
     * Title  : The Alchemist
     * Author : Paulo Coelho
     * Year   : 1988
     * Publisher: HarperCollins
     * Note   : Used here solely for educational/analysis purposes.
     */
    const char *sample =
        "The boy's name was Santiago. He had learned to read from a priest "
        "who had taught him Latin, Spanish, and theology. Since the age of "
        "fifteen, he had traveled the world. He had learned that one person "
        "does not need to know everything, only the things that concern him. "
        "He had crossed deserts, cities, and the mountains of the world. "
        "But here, in this oasis, everything seemed peaceful and familiar.";

    /* dynamically allocate memory for the text */
    int   text_len = (int)strlen(sample) + 1;
    char *text     = malloc(text_len);
    if (!text) { fprintf(stderr, "malloc failed\n"); return 1; }
    strcpy(text, sample);   /* strcpy demonstrated */

    printf("  Text buffer stored at memory address : %p\n", (void *)text);

    /* tokenise the text */
    char **words = NULL;
    int    n     = tokenise(text, &words);

    printf("  Word array stored at memory address  : %p\n\n", (void *)words);

    /* build word statistics */
    struct wordStat *stats = NULL;
    int unique = build_stats(words, n, &stats);

    printf("  Stats array stored at memory address : %p\n\n", (void *)stats);

    /* function pointer array for analysis functions */
    typedef void (*WordFn)(char **, int);
    typedef void (*StatFn)(struct wordStat *, int);

    printf("--- Analysis Results ---\n");
    WordFn wfns[]    = { count_words, longest_word, average_word_length };
    const char *wlabels[] = { "Word Count", "Longest Word", "Avg Word Length" };
    int wn = 3;

    for (int i = 0; i < wn; i++) {
        printf("  [%s]\n", wlabels[i]);
        wfns[i](words, n);   /* call via function pointer */
        printf("\n");
    }

    printf("  [Most Frequent Word]\n");
    StatFn sfn = most_frequent;
    sfn(stats, unique);   /* call via function pointer */
    printf("\n");

    /* interactive menu */
    int choice;
    do {
        printf("--- MENU ---\n");
        printf("  1. Word Count\n");
        printf("  2. Longest Word\n");
        printf("  3. Most Frequent Word\n");
        printf("  4. Average Word Length\n");
        printf("  5. Enter custom text\n");
        printf("  0. Exit\n");
        printf("Choice: ");
        scanf("%d", &choice);
        getchar();   /* consume newline */

        if      (choice == 1) count_words(words, n);
        else if (choice == 2) longest_word(words, n);
        else if (choice == 3) most_frequent(stats, unique);
        else if (choice == 4) average_word_length(words, n);
        else if (choice == 5) {
            /* free previous data before loading new text */
            for (int i = 0; i < n; i++) free(words[i]);
            free(words);
            free(stats);
            free(text);

            printf("  Enter text (max 1000 chars): ");
            text = malloc(1024);
            fgets(text, 1023, stdin);

            n      = tokenise(text, &words);
            unique = build_stats(words, n, &stats);
            printf("  Text loaded. Word count: %d\n", n);
        }
        printf("\n");
    } while (choice != 0);

    /* free all dynamic memory */
    for (int i = 0; i < n; i++) free(words[i]);
    free(words);
    free(stats);
    free(text);

    printf("Goodbye.\n");
    return 0;
}

