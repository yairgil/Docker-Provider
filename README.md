# About

This repository contains source code for Azure Monitor for containers Linux and Windows Agent

# Questions?

Feel free to contact engineering team owners in case you have any questions about this repository or project.

# Prerequisites

1. [Visual Studio Code](https://code.visualstudio.com/) for authoring.
2. [Go lang](https://golang.org/) for building go code
3. Ubuntu machine Ubuntu 14.04 to build Linux Agent
4. [Docker](https://docs.docker.com/engine/install/ubuntu/) to build the docker image for Linux Agent
5. Windows machine to build  Windows Agent
6. [Dokcer for Windows](https://docs.docker.com/docker-for-windows/) to build docker image for Windows Agent

# Repo structure

The general directory structure is:

```
├── build/
|   ├── docker.version              - docker provider version
|   ├── Makefle                     - Makefile to build docker provider code
|   ├── Makefle.common              - dependency common file for Makefile
|   ├── configure                   - configure file to determine provider configuration
|   ├── installer/                  - files related to installer
├── alerts/                         - alert queries
├── kubernetes/                     - files related to Linux and Windows Agent for Kubernetes
│   ├── Linux/                      - scripts to build the Docker image for Linux Agent
│   ├── Windows/                    - scripts to build the Docker image for Windows Agent
│   ├── .../                        - yamls and configuration files to install the Azure Monitor for containers on K8s cluster(s)
├── onboardingscripts/              - scripts for onboarding for Azure Monitor for containers agent to non-AKS clusters
├── source/                         - source code
│   ├── code/                       - source code
│   │   ├── go/                     - plugins code in go
│   │   ├── plugin/                 - plugins code inr ruby
│   │   |   ├── health/             - code for health feature
│   │   |   ├── lib/                - lib for app insights ruby
│   │   ├── toml-parser/            - code for parsing of toml configuration files
│   ├── test/                       - source code for tests
│   │   ├── health/                 - source code for health feature tests
├── troubleshoot/                   - scripts for troubleshooting of Azure Monitor for containers onboarding issues
├── !_README.md                     - this file
├── .gitignore                      - git config file with include/exclude file rules
├── LICENSE                         - License file
├── Rakefile                        - Rake file to trigger ruby plugin tests
└── ReleaseNotes.md                 - Release notes for the release of the Azure Monitor for containers agent
```

# Branches

- `ci_prod` branch contains codebase currently in production (or being prepared for release).
- `ci_dev` branch contains version in development.

To contribute: create your private branch off of `ci_dev`, make changes and use pull request to merge back to `ci_dev`.
Pull request must be approved by at least one engineering team members.

# Authoring code

We recommend using [Visual Studio Code](https://code.visualstudio.com/) for authoring.


# Building code

## Linux Agent

## Build Docker Provider Shell Bundle

1. Begin by downloading the latest package for Go by running this command, which will pull down the Go package file, and save it to your current working directory

```
sudo curl -O https://storage.googleapis.com/golang/go1.9.1.linux-amd64.tar.gz
```
2. Next, use tar to unpack the package. This command will use the tar tool to open and expand the downloaded file, and creates a folder using the package name, and then moves it to $HOME
```
sudo tar -xvf go1.9.1.linux-amd64.tar.gz
sudo mv go ~
```
3. Set PATH, GOBIN, GOPATH  environment variables
```
export PATH=$PATH:$HOME/go/bin
export GOBIN=$HOME/go/bin
export GOPATH=~/Docker-Provider/source/code/go #Set this based on your repo path
```
4. Install glide to manage the go dependencies
```
sudo chmod 777 ~/go/bin #Required to get permissions to create glide executable
curl https://glide.sh/get | sh
cd ~/Docker-Provider/source/code/go/src/plugins #cd <path to go src>
glide init
glide update
glide install
```
> Note: If glide init fails with [ERROR] Cowardly refusing to overwrite existing YAML, you can ignore this error if you havent added any new dependecy else delete glide.yaml and re-run the glide init command.

you can ignore [ERROR] Cowardly refusing to overwrite existing YAML  for glide init.
5.  Build the code  with below commands

```
cd ~/Docker-Provider/build
./configure --enable-ulinux
make
```

## Build Docker Image

1.  Navigate to below directory to build the docker image
  ```
  cd ~/Docker-Provider/kubernetes/linux/
  ```
2. Make sure the latest docker-cimprov-x.x.x-x.universal.x86_64.sh file  location updated in setup.sh
 > Note x.x.x.x is the version of the docker provider which is determined from version in docker.version file
3. Build the Docker image via below command
 ```
   docker build -t  <repo>:<imagetag> .
```
4. Push the Docker image to docker repo

## Windows Agent
TBD