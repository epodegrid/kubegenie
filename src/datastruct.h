//
// Created by epodegrid on 5/20/22.
//

#ifndef K8SGENIE_DATASTRUCT_H
#define K8SGENIE_DATASTRUCT_H

#include <jansson.h>
#include <stdbool.h>

typedef char *path_t;
typedef char *name_t;
typedef char *url_t;
typedef char *pkgID_t;

struct K8S_DATA {
    json_t *root; // Required for all types
    json_error_t error;
    bool isHelm;
    bool isDocker;
    path_t path;
    name_t pkgName;
    pkgID_t pkgID;

    url_t repoURL; //Used for helm
    char *revision;

    path_t sysPath; //Used for k8s
    bool isFile;
    bool postClean;
};

struct CKV_LOG {
    json_t *root;
    json_error_t error;
};

extern struct K8S_DATA K8S_DATA;
extern struct CKV_LOG CKV_LOG;

char *mkrndstr(size_t length);

#endif //K8SGENIE_DATASTRUCT_H