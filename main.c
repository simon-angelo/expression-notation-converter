#include <stdio.h>
#include <string.h>

typedef enum {
    INFIX,
    PREFIX,
    POSTFIX
} Notation;

int main(int argc, char *argv[]) {   
    for (int i = 0; i < argc; i++) {
        printf("Argument %d: %s\n", i, argv[i]);
    }

    char *notationFlag = argv[2];
    Notation targetNotation;
    if (strcmp(notationFlag, "--infix") == 0) {
        targetNotation = INFIX;
    } else if (strcmp(notationFlag, "--prefix") == 0) {
        targetNotation = PREFIX;
    } else if (strcmp(notationFlag, "--postfix") == 0) {
        targetNotation = POSTFIX;
    } else {
        printf("\nSpecified target notation is invalid!");
        printf("\nPlease choose from:");
        printf("\n   --infix");
        printf("\n   --prefix");
        printf("\n   --postfix");
        printf("\n\n");
    }
    return 0;
}