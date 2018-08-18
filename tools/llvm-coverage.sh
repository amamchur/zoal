#!/usr/bin/env bash

cd ./cmake-build-debug-test
rm -rf ../coverage
llvm-profdata-6.0 merge -o ./default.profdata ./default.profraw
llvm-cov-6.0 show -name=MCURDK -instr-profile=default.profdata ./MCURDK_TESTS -format=html -output-dir=../coverage
