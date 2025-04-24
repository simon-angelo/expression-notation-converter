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

Notation detectNotation(const char *expr);
TreeNode* createTreeNode(char *value);
int isOperator(char ch);
int getPrecedence(char op);
int isRightAssociative(char op);
char *notation(Notation not);

void printTreeInOrder(TreeNode *root);
void printTreePreOrder(TreeNode *root);
void printTreePostOrder(TreeNode *root);

TreeNode* buildTreeFromInfix(char *expr);
TreeNode* buildPrefixHelper(char **tokens, int *index, int tokenCount);
TreeNode* buildTreeFromPrefix(char *expr);
TreeNode* buildTreeFromPostfix(char *expr);

int main(int argc, char *argv[]) { 
    if (argc == 1) {
        printf("No arguements passed!\n");
        return 0;
    }
    
    char *expression = argv[1];
    char *notationFlag = argv[2];

    Notation originalNotation;
    if (strcmp(notationFlag, "--infix") == 0) {
        originalNotation = INFIX;
    } else if (strcmp(notationFlag, "--prefix") == 0) {
        originalNotation = PREFIX;
    } else if (strcmp(notationFlag, "--postfix") == 0) {
        originalNotation = POSTFIX;
    } else {
        printf("\nPlease choose from:");
        printf("\n   --infix");
        printf("\n   --prefix");
        printf("\n   --postfix");
        printf("\n\n");
    }   
    
    TreeNode *root = NULL;
    switch (originalNotation) {
        case INFIX:
            root = buildTreeFromInfix(expression);
            break;
        case PREFIX:
            root = buildTreeFromPrefix(expression);
            break;
        case POSTFIX:
            root = buildTreeFromPostfix(expression);
            break;
    }

    printf("\n----------------------------------------------");
    printf("\n\n Expression Notation Converter\n\n");
    printf("----------------------------------------------\n");

    printf("\n[ ORIGINAL EXPRESSION ]");
    switch (originalNotation) {
        case INFIX:
            printf("\n\tInfix: ");
            printf("%s", expression);
            break;
        case PREFIX:
            printf("\n\tPrefix: ");
            printTreePreOrder(root);
            break;
        case POSTFIX:
            printf("\n\tPostfix: ");
            printTreePostOrder(root);
            break;
    }

    printf("\n\n[ CONVERTED EXPRESSIONS ]");
    switch (originalNotation) {
        case INFIX:
            printf("\n\tPrefix: ");
            printTreePreOrder(root);
            printf("\n\tPostfix: ");
            printTreePostOrder(root);
            break;
        case PREFIX:
            printf("\n\tInfix: ");
            printTreeInOrder(root);
            printf("\n\tPostfix: ");
            printTreePostOrder(root);
            break;
        case POSTFIX:
            printf("\n\tPrefix: ");
            printTreePreOrder(root);
            printf("\n\tInfix: ");
            printTreeInOrder(root);
            break;
    }

    printf("\n\n");
    return 0;
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
char *notation(Notation not) {
    switch (not) {
        case INFIX:
            return "Infix";
        case PREFIX:
            return "Prefix";
        case POSTFIX:
            return "Postfix";
    }
}

void printTreeInOrder(TreeNode *root) {
    if (root == NULL) return;

    int needsParens = isOperator(root->value[0]) && strlen(root->value) == 1;

    if (needsParens) printf("(");

    if (root->left) {
        printTreeInOrder(root->left);
        printf(" ");
    }

    printf("%s", root->value);

    if (root->right) {
        printf(" ");
        printTreeInOrder(root->right);
    }

    if (needsParens) printf(")");
}
void printTreePreOrder(TreeNode *root) {
    if (root == NULL) return;
    printf("%s ", root->value);
    printTreePreOrder(root->left);
    printTreePreOrder(root->right);
}
void printTreePostOrder(TreeNode *root) {
    if (root == NULL) return;
    printTreePostOrder(root->left);
    printTreePostOrder(root->right);
    printf("%s ", root->value);
}

TreeNode* buildTreeFromInfix(char *expr) {
    char operatorStack[100];
    int opTop = -1;

    char postfix[100][10];
    int postIndex = 0;

    int i = 0;
    while (expr[i] != '\0') {
        if (isspace(expr[i])) {
            i++;
            continue;
        }

        if (isalnum(expr[i])) {
            char buffer[10];
            int j = 0;
            while (isalnum(expr[i])) {
                buffer[j++] = expr[i++];
            }
            buffer[j] = '\0';
            strcpy(postfix[postIndex++], buffer);
        } else if (expr[i] == '(') {
            operatorStack[++opTop] = expr[i++];
        } else if (expr[i] == ')') {
            while (opTop >= 0 && operatorStack[opTop] != '(') {
                char op[2] = {operatorStack[opTop--], '\0'};
                strcpy(postfix[postIndex++], op);
            }
            if (opTop >= 0 && operatorStack[opTop] == '(') opTop--;
            i++;
        } else if (isOperator(expr[i])) {
            while (
                    opTop >= 0 && 
                    isOperator(operatorStack[opTop]) &&
                    (
                        getPrecedence(operatorStack[opTop]) > getPrecedence(expr[i]) ||
                        (
                            getPrecedence(operatorStack[opTop]) == getPrecedence(expr[i]) &&
                            !isRightAssociative(expr[i])
                        )
                    )
                ) {
                char op[2] = {operatorStack[opTop--], '\0'};
                strcpy(postfix[postIndex++], op);
            }
            operatorStack[++opTop] = expr[i++];
        } else {
            printf("Invalid character: %c\n", expr[i]);
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