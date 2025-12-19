#include <stdio.h>
#include <string.h>

int
main(int argc, char *argv[]) {
    if (argc != 3) {
        printf("usage: %s <pattern> <string>\n", argv[0]);
        return 1;
    }

    char *pattern = argv[1];
    char *text = argv[2];

    if (strstr(text, pattern) != NULL) {
        printf("match found!\n");
        return 0;
    } else {
        printf("no match.\n");
        return 1;
    }
}