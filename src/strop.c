//
// Created by epodegrid on 5/20/22.
//

#include <string.h>
#include <stdlib.h>
#include "strop.h"

char *strm2(char *a, char *b) {
    int size = strlen(a) + strlen(b) + 1;
    char *str = malloc(size);
    strcpy(str, a);
    strcat(str, b);

    return str;
}

char *strm3(char *a, char *b, char *c) {
    int size = strlen(a) + strlen(b) + strlen(c)+ 1;
    char *str = malloc(size);
    strcpy(str, a);
    strcat(str, b);
    strcat(str, c);

    return str;
}

char *strm4(char *a, char *b, char *c, char *d) {
    int size = strlen(a) + strlen(b) + strlen(c) + strlen(d) + 1;
    char *str = malloc(size);
    strcpy(str, a);
    strcat(str, b);
    strcat(str, c);
    strcat(str, d);

    return str;
}

char *strm5(char *a, char *b, char *c, char *d, char *e) {
    int size = strlen(a) + strlen(b) + strlen(c) + strlen(d) + strlen(e) + 1;
    char *str = malloc(size);
    strcpy(str, a);
    strcat(str, b);
    strcat(str, c);
    strcat(str, d);
    strcat(str, e);

    return str;
}

char *strm6(char *a, char *b, char *c, char *d, char *e, char *f) {
    int size = strlen(a) + strlen(b) + strlen(c) + strlen(d) + strlen(e) + strlen(f) +1;
    char *str = malloc(size);
    strcpy(str, a);
    strcat(str, b);
    strcat(str, c);
    strcat(str, d);
    strcat(str, e);
    strcat(str, f);

    return str;
}

char *strm7(char *a, char *b, char *c, char *d, char *e, char *f, char *g) {
    int size = strlen(a) + strlen(b) + strlen(c) + strlen(d) + strlen(e) + strlen(f) + strlen(g) +1;
    char *str = malloc(size);
    strcpy(str, a);
    strcat(str, b);
    strcat(str, c);
    strcat(str, d);
    strcat(str, e);
    strcat(str, f);
    strcat(str, g);

    return str;
}