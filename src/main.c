#include <stdio.h>
#include <string.h>
#include <stdbool.h>

static bool
match_character(char pattern_char, char text_char) {
    if (pattern_char == '.') {
        return true;  /* . matches anything */
    }

    return pattern_char == text_char;
}

static bool
match_pattern(const char *pattern, const char *text) {
    for (int i = 0; pattern[i] != '\0'; i++) { /* foreach char in pattern */
        if (text[i] == '\0') { /* ran out of text but still have pattern left */
            return false;
        }

        if (!match_character(pattern[i], text[i])) {
            return false;
        }
    }

    return true;
}

int
main(int argc, char *argv[]) {
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
