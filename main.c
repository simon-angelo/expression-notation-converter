#include <stdio.h>
#include <string.h>

#define MAX_TREE_NODE_VALUE_LENGTH 10

typedef enum {
    INFIX,
    PREFIX,
    POSTFIX
} Notation;

typedef struct TreeNode {
    char value[MAX_TREE_NODE_VALUE_LENGTH];
    struct TreeNode *left;
    struct TreeNode *right;
} TreeNode;

int isOperator(char ch);
Notation detectNotation(const char *expr);

int main(int argc, char *argv[]) {   
    char *currentNotationString = argv[1];
    char *notationFlag = argv[2];

    for (int i = 0; i < argc; i++) {
        printf("Argument %d: %s\n", i, argv[i]);
    }

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
    
    Notation currentNotation = detectNotation(currentNotationString);

    printf("\nCurrent: %d\nConvert to: %d\n\n", currentNotation, targetNotation);

    return 0;
}

int isOperator(char ch) {
    return ch == '+' || ch == '-' || ch == '*' || ch == '/' || ch == '^';
}

Notation detectNotation(const char *expr) {
    char first, last;

    while (*expr == ' ') {
        expr++;
    }
    first = *expr;
    if (isOperator(first)) {
        return PREFIX;
    }

    const char *end = expr + strlen(expr) - 1;
    while (end > expr && *end == ' ') {
        end--;
    }
    last = *end;
    
    if (isOperator(last)) {
        return POSTFIX;
    } else {
        return INFIX;
    }
}