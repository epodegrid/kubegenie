//
// Created by epodegrid on 5/21/22.
//

#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <sys/param.h>
#include "strop.h"
#include "datastruct.h"
#include "io.h"
#include "error.h"

void print_config(){
    fprintf(stdout, "Assigned tag: %s\n", K8S_DATA.pkgID);

    fprintf(stdout, "Environment Setting <default runtime> : %s\n", K8S_DATA.isDocker?"docker":"runtime");

    if(K8S_DATA.isHelm){
        fprintf(stdout, "Chart checkout revision <default latest> : %s\n", K8S_DATA.revision);
    }
}

char *os_arch(){
    #ifdef __linux__
        #ifdef __x86_64__
            return "yq_linux_amd64";
        #elif __aarch64__
            return "yq_linux_arm64";
        #endif
    #elif __APPLE__
        #ifdef __x86_64__
            return "yq_darwin_amd64";
        #elif __aarch64__
            return "yq_darwin_arm64";
        #endif
    #elif __FreeBSD__
        #ifdef __x86_64__
            return "yq_freebsd_amd64";
        #elif __aarch64__
            return "yq_freebsd_arm64";
        #endif
    #elif __OpenBSD__
            return "yq_openbsd_amd64";
    #elif __NetBSD__
            return "yq_netbsd_amd64";
    #endif
}

void helm_pull() {
    char *cmd = strm5("helm pull ", K8S_DATA.pkgName, " --repo ", K8S_DATA.repoURL, " -d .cache --untar");

    if (strcmp(K8S_DATA.revision, "latest") != 0){
        char *pullVersion = malloc(strlen(cmd));
        strcpy(pullVersion, cmd);

        cmd = malloc(strlen(pullVersion) + strlen(" --version ") + strlen(K8S_DATA.revision) + 1);
        cmd = strm3(pullVersion, " --version ", K8S_DATA.revision);
    }

    if (system(cmd) != 0) {
        error_handler(-1);
    }
}

void helm_render() {
    char *cmd = strm5("helm template .cache/", K8S_DATA.pkgName, " > .cache/", K8S_DATA.pkgID,".yaml");

    if (system(cmd) != 0) {
        error_handler(-2);
    }
}

void dir_render(){
    if (!K8S_DATA.isFile){
        char *cmd = strm5("helm template ", K8S_DATA.sysPath, " > .cache/", K8S_DATA.pkgID,".yaml");

        if (system(cmd) != 0) {
            error_handler(-3);
        }
    }
}

void yq_json(){
    char *filename;

    if (K8S_DATA.isHelm || !K8S_DATA.isFile) {
        filename = malloc(strlen(".cache/") + strlen(K8S_DATA.pkgID) + strlen(".yaml"));
        filename = strm3(".cache/", K8S_DATA.pkgID, ".yaml");
    }
    else {
        filename = malloc(strlen(K8S_DATA.sysPath));
        strcpy(filename,K8S_DATA.sysPath);
    }

    char *cmd = strm7("src/yq/",os_arch(), " ea '[.]' -o=json ", filename, " > .cache/", K8S_DATA.pkgID, ".json");

    if (system(cmd) != 0) {
        error_handler(-4);
    }
}

void k8s_loadf(){
    char *fileName = strm3(".cache/", K8S_DATA.pkgID,".json");

    K8S_DATA.root = json_load_file(fileName, 0, &K8S_DATA.error);

    if (!K8S_DATA.root) // CODE SNIPPET FROM JANSSON EXAMPLE
    {
        fprintf(stderr, "error: on line %d: %s\n", K8S_DATA.error.line, K8S_DATA.error.text);
        error_handler(-5);
    }

    if (!json_is_array(K8S_DATA.root)) // CODE SNIPPET FROM JANSSON EXAMPLE
    {
        json_decref(K8S_DATA.root);
        error_handler(-6);
    }
}


void k8s_flushf(){
    char *fileName = strm7("output/", K8S_DATA.pkgName, "-", K8S_DATA.pkgID, "/", K8S_DATA.pkgName, ".json");

    if (json_dump_file(K8S_DATA.root, fileName, JSON_INDENT(2)) != 0) {
        error_handler(-10);
    }
}

void yq_yaml(){

    char *inFile = strm7("output/", K8S_DATA.pkgName, "-", K8S_DATA.pkgID, "/", K8S_DATA.pkgName, ".json");
    char *outFile = strm7("output/", K8S_DATA.pkgName, "-", K8S_DATA.pkgID, "/", K8S_DATA.pkgName, ".yaml");

    char *cmd = strm6("src/yq/",os_arch()," eval-all -P \'.[] | split_doc\' ", inFile," > ", outFile);

    if (system(cmd) != 0) {
        error_handler(-11);
    }
}