//
// Created by epodegrid on 5/22/22.
//

#ifndef KUBEGENIE_GENIE_H
#define KUBEGENIE_GENIE_H


#include <jansson.h>

void genie_run();
size_t evil_genie(char *resource_name, char *resource_namespace, char *resource_kind);
void good_genie(char *ckv_id, size_t k8s_index, char *kind, char *path);

#endif //KUBEGENIE_GENIE_H
