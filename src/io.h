//
// Created by epodegrid on 5/21/22.
//

#ifndef K8SGENIE_IO_H
#define K8SGENIE_IO_H

void print_config();

void helm_pull();
void helm_render();

void dir_render();
void k8s_loadf();

char *os_arch();
void yq_json();

void k8s_flushf();
void yq_yaml();

#endif //K8SGENIE_IO_H
