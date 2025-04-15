#include <stdio.h>
#include <stdlib.h>
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

TreeNode* createTreeNode(char *value);
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

TreeNode* createTreeNode(char *value) {
    if (strlen(value) >= MAX_TREE_NODE_VALUE_LENGTH) {
        fprintf(stderr, "Error: value too long for TreeNode (max 9 characters).\n");
        exit(-1);
    }

    TreeNode *newNode = (TreeNode *)malloc(sizeof(TreeNode));
    if (newNode == NULL) {
        fprintf(stderr, "Error: failed to allocate TreeNode!\n");
        exit(-2);
    }

    strcpy(newNode->value, value);
    newNode->left = NULL;
    newNode->right = NULL;
    return newNode;
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