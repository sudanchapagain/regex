#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include "../include/regex.h"

static bool
match_star(const char c, const char *pattern, const char *text) {
    if (match_here(pattern, text)) { /* matching zero occurrences first */
        return true;
    }

    while (*text != '\0' && (c == '.' || c == *text)) { /* matching one or more occurrences */
        text++;

        if (match_here(pattern, text)) {
            return true;
        }
    }
    
    return false;
}

static bool
match_here(const char *pattern, const char *text) {
    if (pattern[0] == '\0') {
        return true; /* empty pattern means we've matched */
    }

    if (pattern[1] == '*') {
        return match_star(pattern[0], pattern + 2, text);
    }
    
    /* text is empty but pattern is not */
    if (text[0] == '\0') {
        return false;
    }
    
    /* match single character or . */
    if (pattern[0] == '.' || pattern[0] == text[0]) {
        return match_here(pattern + 1, text + 1);
    }
    
    return false;
}

static bool
match_pattern(const char *pattern, const char *text) {
    do {
        if (match_here(pattern, text)) {
            return true;
        }
    } while (*text++ != '\0');

    return false;
}

int
main(const int argc, const char* const argv[]) {
    if (argc != 3) {
        printf("usage: %s <pattern> <string>\n", argv[0]);
        return 1;
    }

    if (match_pattern(argv[1], argv[2])) {
        printf("match found!\n");
        return 0;
    } else {
        printf("no match.\n");
        return 1;
    }
}
