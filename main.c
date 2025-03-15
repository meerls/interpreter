#include <stdio.h>
#include <stdlib.h>

typedef enum {
    ADD=1, SUB, MUL, DIV, FUN, LET
} type;



typedef struct syntree {
    struct syntree *n[256];
    type t;
} syntree;

void syntree_add(unsigned char *s, type t, syntree *tree) {
    for (; *s; s++) {
        if (!tree->n[*s])
            tree->n[*s] = calloc(1, sizeof(syntree));
        
        tree = tree->n[*s];
    }
    tree->t = t;
}

void syntree_add_file(FILE *f, type t, syntree *tree) {
    int c;
    while ((c = fgetc(f)) == ' ');

    do { 
        if (!tree->n[c])
            tree->n[c] = calloc(1, sizeof(syntree));
        
        tree = tree->n[c];
    } while ((c = fgetc(f)) != ' ');

    tree->t = t;
}

void syntree_destroy(syntree *tree) {
    if (!tree)
        return;

    for (int i = 0; i < 256; i++)
        syntree_destroy(tree->n[i]);

    free(tree);
}

type parse_word(FILE *f, syntree *tree) {
    int c;

    while ((c = fgetc(f)) == ' ');

    do {
        if (!tree->n[c])
            return 0;

        tree = tree->n[c];
    } while ((c = fgetc(f)) != ' ');

    return tree->t;
}


int main(int argc, char **args) {
    if (argc != 2) {
        printf("give one file to run\n");
        return 1;
    }

    FILE *f = fopen(args[1], "r");
    if (!f) {
        printf("file not found\n");
        return 1;
    }

    syntree *tree = calloc(1, sizeof(syntree));
    syntree_add((unsigned char*)"fun", FUN, tree);
    syntree_add((unsigned char*)"let", LET, tree);

    syntree_destroy(tree);
    fclose(f);
    return 0;
}
