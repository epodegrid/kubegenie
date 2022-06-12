//
// Created by epodegrid on 5/22/22.
//

#include <string.h>
#include <malloc.h>
#include "datastruct.h"
#include "switchs.h"
#include "genie.h"

void genie_run() {

    json_t *results = json_object_get(CKV_LOG.root, "results");

    size_t ckv_index;
    json_t *ckv_object;

    json_array_foreach(json_object_get(results, "failed_checks"), ckv_index, ckv_object) {

        json_t *pathObj = json_object_get((json_object_get(ckv_object, "check_result")), "evaluated_keys");
        char *path;

        if (json_array_size(pathObj) != 0){
            path = malloc(strlen(json_string_value(json_array_get(pathObj, 0))) + 1);
            strcpy(path, json_string_value(json_array_get(pathObj, 0)));
        }
        else {
            path = malloc(strlen("spec/template/spec/containers/[0]/") + 1);
            strcpy(path, "spec/template/spec/containers/[0]/");
        }

        char *temp = strtok((char *) json_string_value(json_object_get(ckv_object, "resource")), ".");
        char *ckv_resources[3];

        for (int i = 0; i < 3; i++) {
            ckv_resources[i] = temp;
            temp = strtok(NULL, ".");
        }

        char *ckv_k8s_kind = malloc(strlen(ckv_resources[0]));
        char *ckv_k8s_namespace = malloc(strlen(ckv_resources[1]));
        char *ckv_k8s_name = malloc(strlen(ckv_resources[2]));

        strcpy(ckv_k8s_kind, ckv_resources[0]);
        strcpy(ckv_k8s_namespace, ckv_resources[1]);
        strcpy(ckv_k8s_name, ckv_resources[2]);

        free(temp);

        size_t k8s_index = evil_genie(ckv_k8s_name, ckv_k8s_namespace, ckv_k8s_kind);
        char *ckv_check_id = (char *) json_string_value(json_object_get(ckv_object, "check_id"));

        good_genie(ckv_check_id, k8s_index, ckv_k8s_kind, path);
    }
}

size_t evil_genie(char *resource_name, char *resource_namespace, char *resource_kind) {

    if (K8S_DATA.cacheMatch != -1){
        const char *k8s_name = json_string_value(json_object_get(json_object_get(json_array_get(K8S_DATA.root, K8S_DATA.cacheMatch), "metadata"), "name"));
        const char *k8s_namespace = json_string_value(
                json_object_get(json_object_get(json_array_get(K8S_DATA.root, K8S_DATA.cacheMatch), "metadata"), "namespace"));
        const char *k8s_kind = json_string_value(json_object_get(json_array_get(K8S_DATA.root, K8S_DATA.cacheMatch), "kind"));
        if ((strcmp(resource_name, k8s_name) == 0) && (strcmp(resource_kind, k8s_kind) == 0)) {
            return K8S_DATA.cacheMatch;
        }
    }

    if (K8S_DATA.cacheMatch < json_array_size(K8S_DATA.root) - 1){

        K8S_DATA.cacheMatch += 1;

        const char *k8s_name = json_string_value(json_object_get(json_object_get(json_array_get(K8S_DATA.root, K8S_DATA.cacheMatch), "metadata"), "name"));
        const char *k8s_namespace = json_string_value(
                json_object_get(json_object_get(json_array_get(K8S_DATA.root, K8S_DATA.cacheMatch), "metadata"), "namespace"));
        const char *k8s_kind = json_string_value(json_object_get(json_array_get(K8S_DATA.root, K8S_DATA.cacheMatch), "kind"));
        if ((strcmp(resource_name, k8s_name) == 0) && (strcmp(resource_kind, k8s_kind) == 0)) {
            return K8S_DATA.cacheMatch;
        }
    }

    size_t k8s_index;
    json_t *k8s_object;

    json_array_foreach(K8S_DATA.root, k8s_index, k8s_object) {
        const char *k8s_name = json_string_value(json_object_get(json_object_get(k8s_object, "metadata"), "name"));
        const char *k8s_namespace = json_string_value(
                json_object_get(json_object_get(k8s_object, "metadata"), "namespace"));
        const char *k8s_kind = json_string_value(json_object_get(k8s_object, "kind"));

        if ((strcmp(resource_name, k8s_name) == 0) && (strcmp(resource_kind, k8s_kind) == 0)) {
            K8S_DATA.cacheMatch = k8s_index;
            return k8s_index;
        }
    }
}

void good_genie(char *ckv_id, size_t k8s_index, char *kind, char *path) {
    json_t *ckv_redir[512];
    ckv_redir[0] = json_array_get(K8S_DATA.root, k8s_index);
    int maxPath = 0;

    char *temp = strtok(path, "/");
    char *path_seperator[512];

    while (temp != NULL) {
        path_seperator[maxPath++] = temp;
        temp = strtok(NULL, "/");
    }

    int index = 0;

    while (index < maxPath) {
        if (!json_is_null(ckv_redir[index]) && ckv_redir[index] != NULL) {
            switch (json_typeof(ckv_redir[index])) {
                case 0 :
                    ckv_redir[index + 1] = json_object_get(ckv_redir[index], path_seperator[index]);
                    break;
                case 1 :
                    if (path_seperator[index][0] == '[' &&
                        path_seperator[index][strlen(path_seperator[index]) - 1] == ']') {
                        char *pos = path_seperator[index] + 1;
                        int num = atoi(pos);

                        ckv_redir[index + 1] = json_array_get(ckv_redir[index], num);
                    }
                    else {
                        int num = atoi(path_seperator[index]);
                        ckv_redir[index + 1] = json_array_get(ckv_redir[index], num);
                    }
                    break;
                case 5 :
                case 6 :
                    break;
                default:
                    printf("%d\n",json_typeof(ckv_redir[index]));
                    fprintf(stderr, "Haven't figured this out yet\n");
            }
            index++;
        } else {
//            printf("Error parsing path. Last known location : %s\n", path_seperator[index]);
            break;
        }
    }

    json_t *k8s_outer_spec = json_object_get(ckv_redir[0], "spec");
    json_t *k8s_template = json_object_get(k8s_outer_spec, "template");
    json_t *k8s_inner_spec = json_object_get(k8s_template, "spec");
    json_t *k8s_inner_spec_containers = json_object_get(k8s_inner_spec, "containers");
    json_t *k8s_inner_spec_cd = json_array_get(k8s_inner_spec_containers, 0);
    json_t *k8s_sec_context = json_object_get(k8s_inner_spec_cd, "securityContext");


    switchs(ckv_id)
            {
            cases("CKV_K8S_2")
                json_object_set(ckv_redir[1], "privileged", json_false());
                break;
            cases("CKV_K8S_3")
                json_object_set(ckv_redir[1], "hostIPC", json_false());
                break;
            cases("CKV_K8S_4")
                json_object_set(ckv_redir[1], "hostNetwork", json_false());
                break;
            cases("CKV_K8S_6")
                json_t *runAsUser = json_string("MustRunAsNonRoot");
                json_object_set(ckv_redir[1], "rule", runAsUser);
                break;
            cases("CKV_K8S_7")
                json_t  *requiredDropCapabilities = json_object_get(ckv_redir[1], "requiredDropCapabilities");
                json_t *allCapabilities = json_string("NET_RAW");
                json_object_set(requiredDropCapabilities, "", allCapabilities);
                break;
            cases("CKV_K8S_10") // Needs more depth search
                json_t *requests = json_object();
                json_t *cpuRequest = json_integer(1);
                json_object_set(requests, "cpu", cpuRequest);
                json_object_set(json_object_get(ckv_redir[6], "resources"), "requests", requests);
                break;
            cases("CKV_K8S_11") // Needs more depth search
                json_t *limits = json_object();
                json_t *cpuRequest = json_integer(1);
                json_object_set(limits, "cpu", cpuRequest);
                json_object_set(json_object_get(ckv_redir[6], "resources"), "requests", limits);
                break;
            cases("CKV_K8S_12") // Needs more depth search
                json_t *memoryRequest = json_integer(512);
                json_object_set(json_object_get(json_object_get(ckv_redir[6], "resources"), "requests"), "memory", memoryRequest);
                break;
            cases("CKV_K8S_13") // Needs more depth search
                json_t *memoryRequest = json_integer(512);
                json_object_set(json_object_get(json_object_get(ckv_redir[6], "resources"), "limits"), "memory", memoryRequest);
                break;
            cases("CKV_K8S_20")
                json_object_set(ckv_redir[6], "allowPrivilegeEscalation", json_false());
                break;
            cases("CKV_K8S_21")
                json_t *k8s_meta = json_object_get(ckv_redir[0], "metadata");
                json_t *k8s_namespace = json_object_get(k8s_meta, "namespace");
                json_string_set(k8s_namespace, K8S_DATA.pkgID);
                break;
            cases("CKV_K8S_22")
                json_object_set(ckv_redir[6], "readOnlyRootFilesystem", json_true());
                break;
            cases("CKV_K8S_28")
                json_t *capabilities = json_object();
                json_object_set(ckv_redir[6], "capabilities", capabilities);

                json_t *drop = json_object();
                json_t *all = json_string("NET_RAW");

                json_object_set(drop, "", all);
                json_object_set(capabilities, "drop", drop);
                break;
            cases("CKV_K8S_31")
                json_t *type;
                if (K8S_DATA.isDocker){
                    type = json_string("DockerDefault");
                }
                else {
                    type = json_string("RuntimeDefault");
                }

                json_t *seccompProfile = json_object();

                json_object_set(seccompProfile, "type", type);
                json_object_set(ckv_redir[6], "seccompProfile", seccompProfile);
                break;
            cases("CKV_K8S_37")
                json_t *capabilities = json_object_get(ckv_redir[6], "capabilities");
                json_t *drop = json_object_get(capabilities, "drop");
                json_object_set(drop, "", json_string("NET_RAW"));
            cases("CKV_K8S_40")
                json_t *K8S_RUN_AS_USER = json_object_get(ckv_redir[6], "runAsUser");
                int randomNumber = rand() % 2001 + 10000;

                json_integer_set(K8S_RUN_AS_USER, randomNumber);
                break;
            defaults
                break;
            } switchs_end;
}