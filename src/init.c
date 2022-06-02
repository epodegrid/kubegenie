//
// Created by epodegrid on 5/20/22.
//
#include <stdlib.h>
#include <sys/stat.h>
#include <string.h>
#include <time.h>
#include "io.h"
#include "runner.h"
#include "init.h"
#include "strop.h"

int S_HANDLER(char *argt, char *argv[], int argc) {
    S_INIT(argt, argv, argc);
    S_RUN();
    S_EXIT();
}

int S_INIT(char *argt, char *argv[], int argc) {
    struct stat st = {0};

    K8S_DATA.postClean = true;

    if (strcmp(argv[argc - 1], "--no-cleanup") == 0){
        K8S_DATA.postClean = false;
    }

    if (stat(".cache", &st) == -1) {
        mkdir(".cache", 0700);
    }

    if (stat("output", &st) == -1) {
        mkdir("output", 0700);
    }

    srand(time(NULL));
    int uidSize = (rand() % (15 - 5 + 1)) + 5;

    K8S_DATA.pkgID = malloc(uidSize + 1);
    strcpy(K8S_DATA.pkgID, mkrndstr(uidSize));


    if (strcmp(argt, "--helm") == 0) {

        K8S_DATA.pkgName = malloc(strlen(argv[0]) + 1);
        strcpy(K8S_DATA.pkgName, argv[0]);

        K8S_DATA.repoURL = malloc(strlen(argv[1]) + 1);
        strcpy(K8S_DATA.repoURL, argv[1]);

        K8S_DATA.isHelm = true;

    } else if (strcmp(argt, "--kube") == 0) {

        K8S_DATA.pkgName = malloc(strlen(argv[2]) + 1);
        strcpy(K8S_DATA.pkgName, argv[2]);

        K8S_DATA.sysPath = malloc(strlen(argv[1]) + 1);
        strcpy(K8S_DATA.sysPath, argv[1]);

        if (strcmp(argv[0], "-f") == 0) {
            K8S_DATA.isFile = true;
        } else {
            K8S_DATA.isFile = false;
        }

        K8S_DATA.isHelm = false;
    }

    K8S_DATA.isDocker = false;
    K8S_DATA.revision = malloc(strlen("latest") + 1);
    strcpy(K8S_DATA.revision, "latest");

    if ((argc >= 4) && strcmp(argt, "--helm") == 0) {
        if (strcmp(argv[2], "--seccomp") == 0) {
            if (strcmp(argv[3], "docker") == 0) {
                K8S_DATA.isDocker = true;
            }
        } else if (strcmp(argv[4], "--seccomp") == 0) {
            if (strcmp(argv[5], "docker") == 0) {
                K8S_DATA.isDocker = true;
            }
        }

        if (strcmp(argv[2], "--revision") == 0) {
            K8S_DATA.revision = malloc(strlen(argv[3]) + 1);
            strcpy(K8S_DATA.revision, argv[3]);
        } else if (strcmp(argv[4], "--revision") == 0) {
            K8S_DATA.revision = malloc(strlen(argv[5]) + 1);
            strcpy(K8S_DATA.revision, argv[5]);
        }
    } else if ((argc == 5) && strcmp(argt, "--kube") == 0) {
        if (strcmp(argv[3], "--seccomp") == 0) {
            if (strcmp(argv[4], "docker") == 0) {
                K8S_DATA.isDocker = true;
            }
        }
    }

    char *output_dir = strm4("output/", K8S_DATA.pkgName, "-", K8S_DATA.pkgID);

    if (stat(output_dir, &st) == -1) {
        mkdir(output_dir, 0700);
    }

    return 0;
}

int S_RUN() {
    print_config();

    if (K8S_DATA.isHelm) {
        helm();
    } else {
        kube();
    }
}

int S_EXIT() {
    struct stat st = {0};

    if (!K8S_DATA.postClean){
        return EXIT_SUCCESS;
    }
    else if (stat(".cache", &st) == 0) {
        system("rm -rf .cache");
    }

    return EXIT_SUCCESS;
}