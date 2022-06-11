# KubeGenie

KubeGenie is a cross platform C program to scan kubernetes configuration files and apply basic fixes.
Currently, specific fixes are handled, as reported by checkov. Fixes are implemented by modifying JSON 
files and adding code wherever needed.

KubeGenie is licensed under the [MIT license](https://opensource.org/licenses/MIT); see LICENSE in the source distribution for details.

## Binaries

Binaries are not currently available for KubeGenie due to limited cross-platform support.

## Building

You can build and use KubeGenie by downloading the source code from the releases page,
or by copying this repository.

### Dependencies

KubeGenie currently requires 3 dependencies to function.
1. [checkov](https://github.com/bridgecrewio/checkov) - Used to scan K8S configs. Generates report used to implement fixes.
2. [jansson](https://github.com/akheron/jansson) - JSON library for C
3. [yq](https://github.com/mikefarah/yq) - Performs YAML-JSON roundtrip. Provided as binary in src/yq. 

Refer to the relevant depedency documentation to build/install them.

In addition, make sure to have `cmake`, `make` and `gcc` installed.

### Instructions

To build KubeGenie from source, use the following steps.

```bash
git clone https://github.com/epodegrid/kubegenie.git
cd kubegenie
cmake .
make 
```

## Usage

KubeGenie supports Kubernetes configuration files in YAML, directories with YAML files, or helm charts to be fetched from source.
The following arguments can be used as modifiers.

```bash
                    --helm : Run kubegenie on helm chart
                 --seccomp : Set seccomp to docker/runtime
                --revision : Use specific chart version

                   --kube : Run kubegenie on kubernetes directory/YAML file
                       -f : Provide path to file. Use with --kube
                       -d : Provide path to directory. Use with --kube

             --no-cleanup : Does not clean up the cache directory.
                            Warning! May crash helm charts.
                            Warning! cache can grow significantly large

                  --clean : Clears all cached data and outputs

             -v --version : Displays the version of program
```

### Run

Here are some examples to run KubeGenie :

```bash
    ./kubegenie --help
    ./kubegenie --helm nginx-inress https://helm.nginx.com/stable
    ./kubegenie --kube -f test.yaml --seccomp docker
```

## Bugs
1. ARM Code doesn't compile. Compiler flag for ARM is not detected and fails to pull the right yq binary.
2. yq binaries are not set to executable before running KubeGenie. This causes the program to crash. Workaround is to use the `chmod +x src/yq/*` before first run.