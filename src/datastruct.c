//
// Created by epodegrid on 5/20/22.
//

#include "datastruct.h"
#include <string.h>

struct K8S_DATA K8S_DATA = {0};
struct CKV_LOG CKV_LOG = {0};

char *mkrndstr(size_t length) {

    static char charset[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
    char *randomString;

    if (length) {
        randomString = malloc(length +1);

        if (randomString) {
            int l = (int) (sizeof(charset) -1);
            int key;
            for (int n = 0;n < length;n++) {
                key = rand() % l;
                randomString[n] = charset[key];
            }
            randomString[length] = '\0';
        }
    }

    return randomString;
}
