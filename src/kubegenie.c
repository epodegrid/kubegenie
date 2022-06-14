#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include "init.h"

void usage();

int main(int argc, char *argv[]) {
    char *version = ("\nKubeGenie\n"
                     "Program version: 3.6\n");

    if (argc >= 4 && argc < 10 && strcmp(argv[1],"--helm") == 0) {
        S_HANDLER("--helm", argv + 2, argc - 2);
    }
    else if (argc >= 5 && argc < 9 &&strcmp(argv[1],"--kube") == 0) {
        S_HANDLER("--kube", argv + 2, argc - 2);
    }
    else if (argc == 2 && (strcmp(argv[1],"-v") == 0 || strcmp(argv[1], "--version") == 0)) {
        printf("%s", version);
    }
    else if (argc == 2 && (strcmp(argv[1],"--clean") == 0)) {
        struct stat st = {0};
        if (stat(".cache", &st) == 0) {
            system("rm -rf .cache");
        }
        if (stat("output", &st) == 0) {
            system("rm -rf output");
        }
    }
    else {
        usage();
    }
    return 0;
}

void usage() {
    printf("Usage: [-h help] [--helm] [--kube] [-v version]\n"
           "\n"
           "    Options:\n"

           "        --helm : Run kubegenie on helm chart\n"

           "        --kube : Run kubegenie on kubernetes directory/YAML file\n"
           "            -f : Provide path to file. Use with --kube\n"
           "            -d : Provide path to directory. Use with --kube\n"

           "     --seccomp : Set seccomp to docker/runtime\n"
           "    --revision : Use specific chart version\n"

           "  --no-cleanup : Does not clean up the cache directory.\n"
           "                 Warning! May crash helm charts.\n"
           "                 Warning! cache can grow significantly large\n"

           "       --clean : Clears all cached data and outputs\n"

    "           -v --version : Displays the version of program in use\n");
}
