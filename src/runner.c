//
// Created by epodegrid on 5/21/22.
//

#include "io.h"
#include "ckv.h"
#include "genie.h"
#include "runner.h"

void helm(){
    helm_pull();
    helm_render();
    yq_json();
    k8s_loadf();

    ckv_scan_init();
    ckv_loadf();

    genie_run();

    k8s_flushf();
    yq_yaml();

    ckv_scan_end();
}

void kube(){
    dir_render();
    yq_json();
    k8s_loadf();

    ckv_scan_init();
    ckv_loadf();

    genie_run();

    k8s_flushf();
    yq_yaml();

    ckv_scan_end();
}