#!/bin/bash

docker build base_image -t davidmichelman/td-agent-base-base_image

docker build fluentd-container -t davidmichelman/agenttelemtest

