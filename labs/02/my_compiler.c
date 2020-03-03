#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>


struct StackElement {
    char c;
    int row;
};

struct Stack {
    int top;
    unsigned capacity;
    struct StackElement** array;
};

struct Stack* createStack(unsigned capacity) {
    struct Stack* stack = (struct Stack*)malloc(sizeof(struct Stack));
    stack->capacity = capacity;
    stack->top = -1;
    stack->array = (struct StackElement**)malloc(stack->capacity * sizeof(struct StackElement));
    return stack;
}

struct StackElement* createStackElement(char c, int row) {
    struct StackElement* element = (struct StackElement*)malloc(sizeof(struct StackElement));
    element->c = c;
    element->row = row;
    return element;
}

bool isFull(struct Stack* stack) {
    return stack->top == stack->capacity - 1;
}

bool isEmpty(struct Stack* stack) {
    return stack->top == -1;
}

void push(struct Stack* stack, struct StackElement *element) {
    if (isFull(stack)) {
        return;
    } else {
        stack->array[++stack->top] = element;
    }
}

struct StackElement* pop(struct Stack* stack) {
    if (isEmpty(stack)) {
        return NULL;
    } else {
        return stack->array[stack->top--];
    }
}

struct StackElement* peek(struct Stack* stack) {
    if (isEmpty(stack)) {
        return NULL;
    } else {
        return stack->array[stack->top];
    }
}

int main(int argc, char *argv[]){

    if (argc < 2) {
        printf("How to use: ./my_compiler.c <file to analyze>");
        return 1;
    }

    
    FILE *fp;
    char c;
    int lineError = 1;
    bool hasErrors = false;
    bool possibleComment = false;
    bool comment = false;
    struct Stack* stack = createStack(100);

    fp = fopen(argv[1], "r");

    if (fp == NULL) {
      printf("Error while opening the file.\n");
      return 1;
    }

    while ((c = fgetc(fp)) != EOF ){
        if (c == '\n'){
            lineError++;
            possibleComment = false;
            comment = false;
        } else if (!comment) {
            if (c == '/') {
                if (possibleComment) {
                    comment = true;
                } else {
                    possibleComment = true;
                }
            } else if (isEmpty(stack)) {
                if (c == '(' || c == '{' || c == '[' || c == '\"' || c == '\'') {
                    struct StackElement* element = createStackElement(c, lineError);
                    push(stack, element);
                } else if (c == ')' || c == '}' || c == ']') {
                    printf("There is an unbalanced %c in this line: %d\n", c, lineError);
                    hasErrors = true;
                }
            } else if (peek(stack)->c == '\"') {
                if (c == '\"') {
                    pop(stack);
                }
            } else if (peek(stack)->c == '\'') {
                if (c == '\'') {
                    pop(stack);
                }
            } else if (peek(stack)->c == '(') {
                if (c == ')') {
                    pop(stack);
                } else if (c == '(' || c == '{' || c == '\"' || c == '\'') {
                    struct StackElement* element = createStackElement(c, lineError);
                    push(stack, element);
                }
            } else if (peek(stack)->c == '{') {
                if (c == '}') {
                    pop(stack);
                } else if (c == '(' || c == '{' || c == '\"' || c == '\'') {
                    struct StackElement* element = createStackElement(c, lineError);
                    push(stack, element);
                }
            } else if (peek(stack)->c == '[') {
                if (c == ']') {
                    pop(stack);
                } else if (c == '(' || c == '{' || c == '\"' || c == '\'') {
                    struct StackElement* element = createStackElement(c, lineError);
                    push(stack, element);
                }
            } else if (c == ')' || c == '}' || c == ']') {
                printf("There is an unbalanced %c in this line: %d\n", c, lineError);
                hasErrors = true;
            } else if (c == '(' || c == '{' || c == '[' || c == '\"' || c == '\'') {
                struct StackElement* element = createStackElement(c, lineError);
                push(stack, element);
            } else {
                printf("%c", c);
            }
        }
    }

    while(!isEmpty(stack)) {
        struct StackElement* e = pop(stack);
        printf("There is an unbalanced %c in this line: %d\n", e->c, e->row);
        hasErrors = true;
    }

    if (!hasErrors) {
        printf("There are no errors\n");
    }

    fclose(fp);

    return 0;
}
