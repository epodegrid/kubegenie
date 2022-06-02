//
// Created by epodegrid on 5/20/22.
//

#include <stdio.h>
#include "init.h"
#include "error.h"

void error_handler(int ERROR_CODE) {

    switch (ERROR_CODE) {
        case -1 :
            fprintf(stderr, "Pulling helm chart failed. Check repository and chart name.");
            break;
        case -2 :
            fprintf(stderr, "Rendering helm chart failed. Possible errors in chart.");
            break;
        case -3 :
            fprintf(stderr, "Rendering helm chart failed. Check directory access. Possible errors in chart.");
            break;
        case -4 :
            fprintf(stderr, "Converting yaml to json failed. Ensure yq is present in src/yq. Change mode to executable.");
            break;
        case -5 :
            fprintf(stderr, "json load object failed. Check syntax of chart with --no-cleanup option.");
            break;
        case -6 :
            fprintf(stderr, "Kubernetes configuration file not in right format.");
            break;
        case -7 :
            fprintf(stderr, "Checkov scan failed. This happens with charts containing \"{ }\". Check chart and checkov installation.");
            break;
        case -8 :
            fprintf(stderr, "Loading checkov json object failed. Check yq and checkov installation.");
            break;
        case -9 :
            fprintf(stderr, "Checkov report not in right format or empty. This happens with charts containing \"{ }\". Check configuration file.");
            break;
        case -10 :
            fprintf(stderr, "Dumping output json failed. Debug json dump with --no-cleanup. Check access to path output");
            break;
        case -11 :
            fprintf(stderr, "Cannot convert output json to yaml. Check yq path, and executable. Debug json dump with --no-cleanup.");
            break;
        case -12 :
            fprintf(stderr, "Checkov scan failed. This happens with charts containing \"{ }\". \nCheck chart and checkov installation. Possible errors with yaml dump");
            break;
        default:
            fprintf(stderr, "Error not handled yet.");
            break;
    }
    K8S_DATA.postClean = true;
    S_EXIT();
    exit(ERROR_CODE);
}