#!/bin/bash

for i in $(seq 1 ${1})
do
	../cmake-build-debug/CustomBench cpu 10000000 1000 &
done
