//
// Created by epodegrid on 5/21/22.
//

#include <string.h>
#include "strop.h"
#include "datastruct.h"
#include "error.h"
#include "ckv.h"

void ckv_scan_init(){
    char *fileName;

    if (K8S_DATA.isHelm || !K8S_DATA.isFile) {
        fileName = malloc(strlen(".cache/") + strlen(K8S_DATA.pkgID) + strlen(".yaml"));
        fileName = strm3(".cache/", K8S_DATA.pkgID, ".yaml");
    }
    else {
        fileName = malloc(strlen(K8S_DATA.sysPath));
        strcpy(fileName,K8S_DATA.sysPath);
    }

    fprintf(stdout, "Scanning file with checkov. Please wait\n");

    char *cmd = strm7("checkov -f ", fileName,
                             " -o json --quiet --compact --skip-download > output/",
                             K8S_DATA.pkgName, "-", K8S_DATA.pkgID, "/INIT_CKV_REPORT.json");


    if (system(cmd) == 0) {
        error_handler(-7);
    }

    fprintf(stdout, "Scan complete. Initial report generated at output/%s-%s/INIT_CKV_REPORT.json\n", K8S_DATA.pkgName, K8S_DATA.pkgID);
}

void ckv_loadf() {
    char *fileName = strm5("output/", K8S_DATA.pkgName,"-",K8S_DATA.pkgID, "/INIT_CKV_REPORT.json");

    CKV_LOG.root = json_load_file(fileName,0, &CKV_LOG.error);

    if (!CKV_LOG.root)
    {
        fprintf(stderr, "error: on line %d: %s\n", CKV_LOG.error.line, CKV_LOG.error.text);
        error_handler(-8);
    }

    if (!json_is_object(CKV_LOG.root))
    {
        json_decref(CKV_LOG.root);
        error_handler(-9);
    }
}

void ckv_scan_end(){
    char *inFile = strm7("output/", K8S_DATA.pkgName, "-", K8S_DATA.pkgID, "/", K8S_DATA.pkgName, ".yaml");

    fprintf(stdout, "Generating new checkov report. Please wait.\n");

    char *cmd = strm7("checkov -f ", inFile,
                      " -o json --quiet --compact --skip-download > output/",
                      K8S_DATA.pkgName, "-", K8S_DATA.pkgID, "/END_CKV_REPORT.json");


    if (system(cmd) == 0) {
        error_handler(-12);
    }

    fprintf(stdout, "Scan complete. Final report generated at output/%s-%s/FINAL_CKV_REPORT.json\n", K8S_DATA.pkgName, K8S_DATA.pkgID);
}