//
// Created by epodegrid on 5/22/22.
//

#ifndef K8SGENIE_GENIE_H
#define K8SGENIE_GENIE_H


#include <jansson.h>

void genie_run();
size_t k8s_resource_match(char *resource_name, char *resource_namespace, char *resource_kind);
void fix_genie(char *ckv_id, size_t k8s_index, char *kind);

#endif //K8SGENIE_GENIE_H
