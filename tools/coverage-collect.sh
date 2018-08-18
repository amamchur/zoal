#!/usr/bin/env bash

cd ./cmake-build-debug-test/CMakeFiles/MCURDK_TESTS.dir/tests
pwd

lcov --capture --directory . --output-file coverage_full.info
lcov --remove coverage_full.info \
    '/usr/include/*' '*/gtest/*' '*/tests/*' '*/v1/*' \
    --output-file coverage.info

genhtml coverage.info -o ../../../../coverage

lcov -z -d .
