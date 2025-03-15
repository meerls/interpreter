#include <stdio.h>
#include <stdlib.h>

// types which the file will be converted to
typedef enum {
    // invalid
    NIL,
    // operators
    ADD, SUB, MUL, DIV,
    // declarations
    FUN, LET,
    // user defined names
    FNAME, VNAME,
} type;

// tree that stores words and makes
// parsing a lot easier
typedef struct syntree {
    struct syntree *n[256];
    type t;
} syntree;

// adds a word to a syntax tree
void syntree_add(unsigned char *s, type t, syntree *tree) {
    for (; *s; s++) {
        if (!tree->n[*s])
            tree->n[*s] = calloc(1, sizeof(syntree));
        
        tree = tree->n[*s];
    }
    tree->t = t;
}

// adds a word from a file
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

// destroys a syntax tree and sets
// the children to null
void syntree_destroy(syntree *tree) {
    if (!tree)
        return;

    for (int i = 0; i < 256; i++) {
        syntree_destroy(tree->n[i]);
        tree->n[i] = NULL;
    }

    free(tree);
}

// reads a word from file and tries to traverse the
// syntax tree, if no matching key word is found
// this will return nil
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
        printf("give me one file\n");
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

    while (1) {
        switch (parse_word(f, tree)) {
            case NIL:

            case FUN:
                syntree_add_file(f, FNAME, tree);
                break;
            case LET:
                syntree_add_file(f, VNAME, tree);
                break;
        }
        printf("%d\n", t);
    }

    syntree_destroy(tree);
    fclose(f);
    return 0;
}
