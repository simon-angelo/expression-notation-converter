#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

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
void printTreeInOrder(TreeNode *root);
int isOperator(char ch);
int getPrecedence(char op);
int isRightAssociative(char op);

Notation detectNotation(const char *expr);
TreeNode* buildInfixTree(char *infix);
TreeNode* buildPrefixHelper(char **tokens, int *index, int tokenCount);
TreeNode* buildTreeFromPrefix(char *expr);
TreeNode* buildTreeFromPostfix(char *expr);

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
void printTreeInOrder(TreeNode *root) {
    if (root == NULL) return;
    
    if (isOperator(root->value[0]) && strlen(root->value) == 1) {
        printf("(");
    }

    printTreeInOrder(root->left);
    printf("%s", root->value);
    printTreeInOrder(root->right);

    if (isOperator(root->value[0]) && strlen(root->value) == 1) {
        printf(")");
    }
}
int isOperator(char ch) {
    return ch == '+' || ch == '-' || ch == '*' || ch == '/' || ch == '^';
}
int getPrecedence(char op) {
    switch (op) {
        case '+':
        case '-': 
            return 1;
        case '*':
        case '/': 
            return 2;
        case '^': 
            return 3;
    }
    return 0;
}
int isRightAssociative(char op) {
    return op == '^';
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

TreeNode* buildInfixTree(char *infix) {
    char operatorStack[100];
    int opTop = -1;

    char postfix[100][10];
    int postIndex = 0;

    int i = 0;
    while (infix[i] != '\0') {
        if (isspace(infix[i])) {
            i++;
            continue;
        }

        if (isalnum(infix[i])) {
            char buffer[10];
            int j = 0;
            while (isalnum(infix[i])) {
                buffer[j++] = infix[i++];
            }
            buffer[j] = '\0';
            strcpy(postfix[postIndex++], buffer);
        } else if (infix[i] == '(') {
            operatorStack[++opTop] = infix[i++];
        } else if (infix[i] == ')') {
            while (opTop >= 0 && operatorStack[opTop] != '(') {
                char op[2] = {operatorStack[opTop--], '\0'};
                strcpy(postfix[postIndex++], op);
            }
            if (opTop >= 0 && operatorStack[opTop] == '(') opTop--;
            i++;
        } else if (isOperator(infix[i])) {
            while (
                    opTop >= 0 && 
                    isOperator(operatorStack[opTop]) &&
                    (
                        getPrecedence(operatorStack[opTop]) > getPrecedence(infix[i]) ||
                        (
                            getPrecedence(operatorStack[opTop]) == getPrecedence(infix[i]) &&
                            !isRightAssociative(infix[i])
                        )
                    )
                ) {
                char op[2] = {operatorStack[opTop--], '\0'};
                strcpy(postfix[postIndex++], op);
            }
            operatorStack[++opTop] = infix[i++];
        } else {
            printf("Invalid character: %c\n", infix[i]);
            exit(1);
        }
    }

    while (opTop >= 0) {
        char op[2] = {operatorStack[opTop--], '\0'};
        strcpy(postfix[postIndex++], op);
    }

    TreeNode* stack[100];
    int top = -1;

    for (int i = 0; i < postIndex; i++) {
        if (isOperator(postfix[i][0]) && strlen(postfix[i]) == 1) {
            TreeNode *node = createTreeNode(postfix[i]);
            node->right = stack[top--];
            node->left = stack[top--];
            stack[++top] = node;
        } else {
            stack[++top] = createTreeNode(postfix[i]);
        }
    }

    return stack[top];
}
TreeNode* buildPrefixHelper(char **tokens, int *index, int tokenCount) {
    if (*index >= tokenCount) return NULL;

    TreeNode *node = createTreeNode(tokens[(*index)++]);

    if (isOperator(node->value[0]) && strlen(node->value) == 1) {
        node->left = buildPrefixHelper(tokens, index, tokenCount);
        node->right = buildPrefixHelper(tokens, index, tokenCount);
    }

    return node;
}
TreeNode* buildTreeFromPrefix(char *expr) {
    char *tokens[100];
    int tokenCount = 0;

    char *token = strtok(expr, " ");
    while (token != NULL) {
        tokens[tokenCount++] = token;
        token = strtok(NULL, " ");
    }

    int index = 0;
    return buildPrefixHelper(tokens, &index, tokenCount);
}
TreeNode* buildTreeFromPostfix(char *expr) {
    TreeNode *stack[100];
    int top = -1;

    char *token = strtok(expr, " ");
    while (token != NULL) {
        if (isOperator(token[0]) && strlen(token) == 1) {
            TreeNode *node = createTreeNode(token);
            node->right = stack[top--];
            node->left = stack[top--];
            stack[++top] = node;
        } else {
            stack[++top] = createTreeNode(token);
        }
        token = strtok(NULL, " ");
    }

    return stack[top];
}
